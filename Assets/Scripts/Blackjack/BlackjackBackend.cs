using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.AI;
public enum Chip
{
    Chip_1 = 1,
    Chip_2 = 2,
    Chip_5 = 5,
    Chip_10 = 10,
    Chip_25 = 25,
    Chip_50 = 50,
    Chip_100 = 100,
    Chip_200 = 200,
    Chip_500 = 500,
    Chip_1000 = 1000
}

public class BlackjackBackend : MonoBehaviour
{
    GameObject playerObject;
    GameObject opponentObject;
    NPCBlackjackAI opponent;
    PlayerInformation player;
    GameObject blackjackUI;
    Deck playerDeck1, playerDeck2, opponentDeck1, opponentDeck2;
    Deck playerDeck, opponentDeck;

    private bool userTurn;
    private bool betPhase;
    public bool roundEnd;
    private bool userSwapped;
    public bool opponentThinking;
    int maxBet;
    int maxChips;
    int roundCount;
    int pot;
    readonly Chip[] chipValues = { Chip.Chip_1, Chip.Chip_2, Chip.Chip_5, Chip.Chip_10, Chip.Chip_25, Chip.Chip_50, Chip.Chip_100, Chip.Chip_200, Chip.Chip_500, Chip.Chip_1000 };

    

    void OnEnable()
    {
        playerObject = GameObject.Find("Player");
        opponentObject = GameObject.FindGameObjectWithTag("Opponent");
        if (opponentObject == null) {
            gameObject.SetActive(false);
        }
        opponent = opponentObject.GetComponent<NPCBlackjackAI>();
        player = playerObject.GetComponent<PlayerInformation>();
        blackjackUI = transform.GetChild(0).gameObject;

        playerDeck1 = new Deck(); playerDeck2 = new Deck(); opponentDeck1 = new Deck(); opponentDeck2 = new Deck();

        playerDeck = playerDeck1.MergeDecks(playerDeck2);
        opponentDeck = opponentDeck1.MergeDecks(opponentDeck2);
        if (playerObject != null) {
            playerObject.SetActive(false);
        }
        userTurn = true;
        betPhase = true;
        roundCount = 0;
        StartGame();

    }

    void OnDisable()
    {
        if (playerObject != null)
        {
            playerObject.SetActive(true);
        }

        // Resets all the variables

        /*
        player.hand.ClearHand();
        player.currentChips.Clear();
        opponent.hand.ClearHand();
        opponent.currentChips.Clear();
        playerDeck.ClearDeck();
        opponentDeck.ClearDeck();
        player = null;
        opponent = null;
        playerObject = null;
        opponentObject.SetActive(false);
        opponentObject = null;
        blackjackUI = null;
        playerDeck1.ClearDeck(); playerDeck2.ClearDeck(); opponentDeck1.ClearDeck(); opponentDeck2.ClearDeck();
        */
        userTurn = false;
        betPhase = false;
        roundEnd = false;
        userSwapped = false;
        opponentThinking = false;
        maxBet = 0;
        maxChips = 0;
        roundCount = 0;
        pot = 0;
         
    }

    // Update is called once per frame
    void Update()
    {
        if (roundEnd)
        {
            return;
        }

        if (!userTurn && opponent.isDealer && !roundEnd)
        {
            // Dealer's turn
            if (!opponentThinking)
            {
                opponentThinking = true;
                Invoke(nameof(DealerPhase), 2.0f);
            }
        }
        else if (!userTurn && !opponent.isDealer && !roundEnd)
        {
            // Opponent's turn
            if (!opponentThinking)
            {
                opponentThinking = true;
                Invoke(nameof(OpponentPhase), 2.0f);
            }
        }
        if (Input.GetMouseButtonDown(0))
        {
            Vector3 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            if (betPhase && userTurn) { 
                for (int i = 0; i <= maxChips; i++) {
                    if (maxChips == 99)
                    {
                        break;
                    }
                    if (blackjackUI.transform.GetChild(2).gameObject.transform.GetChild(i).GetComponent<CircleCollider2D>().OverlapPoint(mousePos)) {  
                        int bet = (int)chipValues[i];
                        if (player.money < bet || opponent.money < bet)
                        {
                            // All in WIP
                            break;
                        }
                        pot += bet;
                        player.money -= bet;
                        player.currentChips.Add(chipValues[i]);

                        pot += bet;
                        opponent.money -= bet;
                        opponent.currentChips.Add(chipValues[i]);
                        if (player.money < opponent.money)
                        {
                            UpdateMaxChips(player.money);
                        }
                        else
                        {
                            UpdateMaxChips(opponent.money);
                        }
                    }
                }
                // Clicking the topmost chip returns it to the player's hand, and returns an equal amount to the player's money. Also returns the opponents money and chips
                if (blackjackUI.transform.GetChild(2).gameObject.transform.GetChild(10).GetComponent<CircleCollider2D>().OverlapPoint(mousePos) && player.currentChips.Count > 0)
                {
                    player.money += (int)player.currentChips.Last();
                    opponent.money += (int)opponent.currentChips.Last();
                    player.currentChips.Remove(player.currentChips.Last());
                    opponent.currentChips.Remove(opponent.currentChips.Last());
                    UpdateMaxChips(player.money);
                }
                if (blackjackUI.transform.GetChild(9).gameObject.GetComponent<CircleCollider2D>().OverlapPoint(mousePos))
                {
                    betPhase = false;
                    ToggleChipInput(betPhase, maxChips);
                    if (player.hand.IsBlackjack())
                        EndRound(true);
                    else if (opponent.hand.IsBlackjack())
                        EndRound(false);

                }
            } else if (!betPhase && userTurn)
            {
                if (blackjackUI.transform.GetChild(8).gameObject.GetComponent<BoxCollider2D>().OverlapPoint(mousePos)) // Hit
                {
                    player.hand.AddCard(playerDeck.DealCard());
                    Invoke(nameof(HandCheck), 0.5f);
                } else if (blackjackUI.transform.GetChild(9).gameObject.GetComponent<CircleCollider2D>().OverlapPoint(mousePos)) // Stand
                {
                    if (player.isDealer)
                        EndRound(player.hand.HandValue() >= opponent.hand.HandValue());
                    else 
                        EndTurn();
                }
                else if (blackjackUI.transform.GetChild(10).gameObject.GetComponent<CircleCollider2D>().OverlapPoint(mousePos) && !player.isDealer) // Double
                {
                    if (player.money < pot / 2)
                    {
                        // All in
                        int allIn = player.money;
                        pot += allIn;
                        player.currentChips = MoneyToChips(allIn);
                        player.money = 0;
                        if (allIn > opponent.money)
                        {
                            allIn = opponent.money;
                        }
                        pot += allIn;
                        opponent.money -= allIn;
                        
                    }
                    int betAmount = pot / 2;
                    pot += betAmount;
                    player.money -= betAmount;
                    List<Chip> chips = MoneyToChips(betAmount);
                    for (int i = 0; i < chips.Count; i++)
                    {
                        player.currentChips.Add(chips[i]);
                    }
                    if (betAmount > opponent.money)
                    {
                        betAmount = opponent.money;
                    }
                    pot += betAmount;
                    opponent.money -= betAmount;
                    chips = MoneyToChips(betAmount);
                    for (int i = 0; i < chips.Count; i++)
                    {
                        player.currentChips.Add(chips[i]);
                    }
                    player.hand.AddCard(playerDeck.DealCard());
                    Invoke(nameof(HandCheck), 0.5f);
                    EndTurn();
                }
            }
        }
        Render();

    }

    void StartGame() {
        betPhase = true;
        maxBet = player.money;
        UpdateMaxChips(maxBet);
        ToggleChipInput(betPhase, maxChips); 
        userTurn = true;
        opponent.isDealer = true;
        roundCount = 0;
        pot = 0;

        playerDeck1 = new Deck(); playerDeck2 = new Deck(); opponentDeck1 = new Deck(); opponentDeck2 = new Deck();
        player.hand = new Hand(); opponent.hand = new Hand();

        playerDeck = playerDeck1.MergeDecks(playerDeck2);
        opponentDeck = opponentDeck1.MergeDecks(opponentDeck2);
        playerDeck.Shuffle();
        opponentDeck.Shuffle();

        StartRound();
    }
    void StartRound()
    {
        roundEnd = false;
        
        if (player.money <= 0 || opponent.money <= 0)
        {
            EndGame();
        }


        if (roundCount != 0)
        {
            player.isDealer = !player.isDealer;
            opponent.isDealer = !opponent.isDealer;
            string name = player.isDealer ? player.name : opponent.name;
            Debug.Log("New Round! dealer is " + name);
            if (player.hand != null)
            {
                playerDeck.Discard(player.hand.Cards);
                opponentDeck.Discard(opponent.hand.Cards);
            }
            player.hand.ClearHand();
            opponent.hand.ClearHand();

            blackjackUI.transform.GetChild(12).transform.rotation = Quaternion.Euler(0, 0, 0);

            if (!userSwapped)
            {
                userTurn = !userTurn;
                userSwapped = true;
                if (!userTurn)
                {
                    blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " is thinking";
                }

            }
        } 
          
        userSwapped = false;

        player.hand.AddCard(playerDeck.DealCard());
        opponent.hand.AddCard(opponentDeck.DealCard());
        player.hand.AddCard(playerDeck.DealCard());
        opponent.hand.AddCard(opponentDeck.DealCard());

        player.currentChips.Clear();
        opponent.currentChips.Clear();
        pot = 0;
        betPhase = true;
        UpdateMaxChips(player.money);
        ToggleChipInput(betPhase, maxChips);

        blackjackUI.transform.GetChild(13).transform.rotation = Quaternion.Euler(0, 0, 0);
        
        if (playerDeck.CardsLeft() < 20)
            playerDeck.Reshuffle();

        if (opponentDeck.CardsLeft() < 20) 
            opponentDeck.Reshuffle();
        

    }
    void Render()
    {


        // Rendering the player's chips
        GameObject playerChips = blackjackUI.transform.GetChild(2).gameObject;

        // Disabling all chips
        for (int i = 0; i <= 10; i++)
        {
            playerChips.transform.GetChild(i).gameObject.SetActive(false);
        }

        // Rendering the player's current chips
        if (maxChips != 99)
        {
            for (int i = 0; i <= maxChips; i++)
            {
                playerChips.transform.GetChild(i).gameObject.SetActive(true);
            }
        }
        // Rendering the player's last betted chip
        if (player.currentChips.Count > 0)
        {  // Sorry, not really easy to understand what this is doing, don't worry I know what it does

            playerChips.transform.GetChild(10).gameObject.GetComponent<SpriteRenderer>().sprite = playerChips.transform.GetChild(System.Array.IndexOf(chipValues, player.currentChips.Last())).GetComponent<SpriteRenderer>().sprite;
            playerChips.transform.GetChild(10).gameObject.SetActive(true);
        }
        // Rendering the opponent's chips
        GameObject opponentChips = blackjackUI.transform.GetChild(5).gameObject;

        // Disabling all chips
        for (int i = 0; i <= 10; i++)
        {
            opponentChips.transform.GetChild(i).gameObject.SetActive(false);
        }

        // Rendering the opponent's current chips
        if (maxChips != 99) 
        { 
            for (int i = 0; i <= maxChips; i++)
            {
                opponentChips.transform.GetChild(i).gameObject.SetActive(true);
            }
        }
        // Rendering the opponent's last betted chip
        if (opponent.currentChips.Count > 0)
        {
            opponentChips.transform.GetChild(10).gameObject.GetComponent<SpriteRenderer>().sprite = opponentChips.transform.GetChild(System.Array.IndexOf(chipValues, player.currentChips.Last())).GetComponent<SpriteRenderer>().sprite;
            opponentChips.transform.GetChild(10).gameObject.SetActive(true);
        }
        // Rendering the player's current hand
        GameObject playerHand = blackjackUI.transform.GetChild(1).gameObject;

        for (int i = 0; i <= 4; i++)
        {
            playerHand.transform.GetChild(i).gameObject.SetActive(false);        // Resets the player's hand rendering
        }
        if (player.hand != null && player.hand.CardsInHand() > 1 && !(betPhase && userTurn))
        {
            for (int i = 0; i < player.hand.Cards.Count; i++)
            {
                playerHand.transform.GetChild(i).GetComponent<SpriteRenderer>().sprite = player.hand.Cards[i].CardSprite;
                playerHand.transform.GetChild(i).gameObject.SetActive(true);
            }
        }

        // Rendering the player discard pile's top card
        if (playerDeck.discardPile.Count > 0)
            blackjackUI.transform.GetChild(15).GetComponent<SpriteRenderer>().sprite = playerDeck.discardPile.Last().CardSprite;
        else
            blackjackUI.transform.GetChild(15).GetComponent<SpriteRenderer>().sprite = null;

        // Rendering the opponent's current hand
        GameObject opponentHand = blackjackUI.transform.GetChild(4).gameObject;

        for (int i = 0; i <= 4; i++)
        {
            opponentHand.transform.GetChild(i).gameObject.SetActive(false);        // Resets the opponent's hand rendering
        }
        if (opponent.hand != null && opponent.hand.CardsInHand() > 1 && !(betPhase && userTurn))
        {
            int j = 0;
            if (opponent.isDealer && userTurn) // If the opponent is the dealer and it's the player's turn, hide the first card
            {
                Sprite backSprite = Resources.Load<Sprite>("Blackjack/Playing_Cards/Card_Back"); 
                opponentHand.transform.GetChild(0).GetComponent<SpriteRenderer>().sprite = backSprite;
                opponentHand.transform.GetChild(0).gameObject.SetActive(true);
                j = 1;
            }
            for (int i = j; i < opponent.hand.Cards.Count; i++)
            {
                opponentHand.transform.GetChild(i).GetComponent<SpriteRenderer>().sprite = opponent.hand.Cards[i].CardSprite;
                opponentHand.transform.GetChild(i).gameObject.SetActive(true);
            }
        }

        // Rendering the opponent discard pile's top card
        if (opponentDeck.discardPile.Count > 0)
            blackjackUI.transform.GetChild(16).GetComponent<SpriteRenderer>().sprite = opponentDeck.discardPile.Last().CardSprite;
        else
            blackjackUI.transform.GetChild(16).GetComponent<SpriteRenderer>().sprite = null;


        // Rendering the player's money
        blackjackUI.transform.GetChild(3).GetComponent<TMPro.TextMeshPro>().text = "$" + player.money.ToString();
        
        // Rendering the opponent's money
        blackjackUI.transform.GetChild(6).GetComponent<TMPro.TextMeshPro>().text = "$" + opponent.money.ToString();

        // Rendering the Oppoenent action
        if (userTurn)
        {
            blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " is waiting";
        }

        // Rendering the player and opponents player/dealer status
        if (player.isDealer)
        {
            blackjackUI.transform.GetChild(13).GetComponent <SpriteRenderer>().sprite = Resources.Load<Sprite>("Blackjack/DealerTurn");
            blackjackUI.transform.GetChild(14).GetComponent <SpriteRenderer>().sprite = Resources.Load<Sprite>("Blackjack/PlayerTurn");
        }
        else
        {
            blackjackUI.transform.GetChild(13).GetComponent<SpriteRenderer>().sprite = Resources.Load<Sprite>("Blackjack/PlayerTurn");
            blackjackUI.transform.GetChild(14).GetComponent<SpriteRenderer>().sprite = Resources.Load<Sprite>("Blackjack/DealerTurn");
        }

    }
    void DealerPhase()
    {
        switch (opponent.DetermineAction(opponent.hand, player.hand))
        {
            case Action.Hit:
                blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Hit";
                Debug.Log(opponent.name + " Hit");
                opponent.hand.AddCard(opponentDeck.DealCard());
                Invoke(nameof(HandCheck), 0.5f);
                break;
            case Action.Stand:
                blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Stood";
                Debug.Log(opponent.name + " Stood");
                Invoke(nameof(HandCheck), 0.5f);
                EndRound(player.hand.HandValue() >= opponent.hand.HandValue());
                break;
            // Dealer cannot Double, split, or take insurance
            case Action.Double:
            case Action.Split:
            case Action.Insurance:
                break;
        }
        opponentThinking = false;
    }
    void OpponentPhase()
    {
        if (betPhase)
        {
            betPhase = false;
            int betAmount = opponent.DetermineBet(); // Sets the bet amount to the opponent's determined bet. Calculated in NPCBlackjackAI.cs
            blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Bet $" + betAmount;
            Debug.Log(opponent.name + " Bet $" + betAmount);

            if (betAmount > player.money)
                betAmount = player.money;

            pot += betAmount;
            opponent.money -= betAmount;
            List<Chip> chips = MoneyToChips(betAmount);
            foreach (Chip chip in chips)
            {
                opponent.currentChips.Add(chip);
            }

            pot += betAmount;
            player.money -= betAmount;
            chips = MoneyToChips(betAmount);
            foreach (Chip chip in chips)
            {
                player.currentChips.Add(chip);
            }
        }
        else
        {
            HandCheck();
            switch (opponent.DetermineAction(opponent.hand, player.hand.Cards[0]))
            {
                case Action.Hit:
                    opponent.hand.AddCard(opponentDeck.DealCard());
                    blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Hit";
                    Debug.Log(opponent.name + " Hit");
                    Invoke(nameof(HandCheck), 0.5f);
                    break;
                case Action.Stand:
                    blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Stood";
                    Debug.Log(opponent.name + " Stood");
                    Invoke(nameof(HandCheck), 0.5f);
                    EndTurn();
                    break;
                case Action.Double:
                    blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Doubled";
                    Debug.Log(opponent.name + " Doubled");
                    if (opponent.money < pot / 2)
                    {
                        // All in
                        int allIn = opponent.money;
                        pot += allIn;
                        opponent.currentChips = MoneyToChips(allIn);
                        opponent.money = 0;
                        if (allIn > player.money)
                        {
                            allIn = player.money;
                        }
                        pot += allIn;
                        player.money -= allIn;
                        break;
                    }
                    int betAmount = pot / 2;
                    pot += betAmount;
                    opponent.money -= betAmount;
                    List<Chip> chips = MoneyToChips(betAmount);
                    for (int i = 0; i < chips.Count; i++)
                    {
                        opponent.currentChips.Add(chips[i]);
                    }
                    if (betAmount > player.money)
                    {
                        betAmount = player.money;
                    }
                    pot += betAmount;
                    player.money -= betAmount;
                    chips = MoneyToChips(betAmount);
                    for (int i = 0; i < chips.Count; i++)
                    {
                        opponent.currentChips.Add(chips[i]);
                    }
                    opponent.hand.AddCard(opponentDeck.DealCard());
                    Invoke(nameof(HandCheck), 0.5f);
                    EndTurn();
                    break;
                case Action.Split:
                    blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Split";
                    Debug.Log(opponent.name + " Split");
                    break;
                case Action.Insurance:
                    blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Insured";
                    Debug.Log(opponent.name + " Insured");
                    break;
            }
        }
        opponentThinking = false;
    }
    void HandCheck() {         
        if (player.hand.IsBust())
        {
            // Player Bust
            blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = player.name + " Bust";
            Debug.Log(player.name + " Bust");
            roundEnd = true;
            EndRound(false);
        }
        else if (player.hand.IsBlackjack())
        {
            // Player Blackjack
            blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = player.name + " got Blackjack!";
            Debug.Log(player.name + " Blackjack");
            roundEnd = true;
            EndRound(true);
        }
        else if (opponent.hand.IsBust())
        {
            // Opponent Bust
            blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Bust";
            Debug.Log(opponent.name + " Bust");
            roundEnd = true;
            EndRound(true);
        }
        else if (opponent.hand.IsBlackjack())
        {
            // Opponent Blackjack
            blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " got Blackjack!";
            Debug.Log(opponent.name + " Blackjack");
            roundEnd = true;
            EndRound(false);
        }
    }
    void EndTurn()
    {
        userTurn = !userTurn; 
        userSwapped = true;
        Debug.Log("End Turn, userTurn is " + userTurn); 
    }
    void EndRound(bool userWin)
    {
        roundCount++;
        if (userWin)
        {
            blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = player.name + " Won!";
            Debug.Log(player.name + " Won! They earned $" + pot + "!");
            player.money += pot;
            blackjackUI.transform.GetChild(12).transform.rotation = Quaternion.Euler(0, 0, 90);
        }
        else
        {
            blackjackUI.transform.GetChild(17).GetComponent<TMPro.TextMeshPro>().text = opponent.name + " Won!";
            Debug.Log(opponent.name + " Won! They earned $" + pot + "!");
            opponent.money += pot;
            blackjackUI.transform.GetChild(12).transform.rotation = Quaternion.Euler(0, 0, -90);
        }
        roundEnd = true;
        opponentThinking = false;
        CancelInvoke();
        Invoke(nameof(StartRound), 3.0f);

    }

    private void EndGame()
    {
        opponentObject.GetComponentInParent<NPCHandler>().playingBlackjack = false;
        opponentObject.tag = "Untagged";
        
        
        gameObject.SetActive(false);
    }

    // Sets the Chip's 2d Circle Colliders to active or inactive. Doesn't disable the sprite renderer or gameObject.
    void ToggleChipInput(bool toggle, int upToChip) {
        if (upToChip == 99)
        {
            return;
        }
        GameObject playerChips = blackjackUI.transform.GetChild(2).gameObject;
        for (int i = 0; i < upToChip; i++) {
            playerChips.transform.GetChild(i).GetComponent<CircleCollider2D>().enabled = toggle;
        }
    }

    List<Chip> MoneyToChips(int amount) {
        List<Chip> chips = new();
        while (amount > 0) {
            if (amount >= 1000) {
                chips.Add(Chip.Chip_1000);
                amount -= 1000;
            }
            else if (amount >= 500) {
                chips.Add(Chip.Chip_500);
                amount -= 500;
            }
            else if (amount >= 200) {
                chips.Add(Chip.Chip_200);
                amount -= 200;
            }
            else if (amount >= 100) {
                chips.Add(Chip.Chip_100);
                amount -= 100;
            }
            else if (amount >= 50) {
                chips.Add(Chip.Chip_50);
                amount -= 50;
            }
            else if (amount >= 25) {
                chips.Add(Chip.Chip_25);
                amount -= 25;
            }
            else if (amount >= 10) {
                chips.Add(Chip.Chip_10);
                amount -= 10;
            }
            else if (amount >= 5) {
                chips.Add(Chip.Chip_5);
                amount -= 5;
                
            }
            else if (amount >= 2) {
                chips.Add(Chip.Chip_2);
                amount -= 2;
                
            }
            else if (amount >= 1) {
                chips.Add(Chip.Chip_1);
                amount -= 1;
                
            }
        }
        return chips;
    }
    void UpdateMaxChips(int money)
    {
        if (money >= 1000)
        {
            maxChips = 9;
        } else if (money >= 500)
        {
            maxChips = 8;
        } else if (money >= 200)
        {
            maxChips = 7;
        } else if (money >= 100)
        {
            maxChips = 6;
        } else if (money >= 50)
        {
            maxChips = 5;
        } else if (money >= 25)
        {
            maxChips = 4;
        } else if (money >= 10)
        {
            maxChips = 3;
        } else if (money >= 5)
        {
            maxChips = 2;
        } else if (money >= 2)
        {
            maxChips = 1;
        } else if (money >= 1)
        {
            maxChips = 0;
        } else if (money == 0)
        {
            maxChips = 99;
        }
        
    }
    int ChipsToMoney(List<Chip> chips)
    {
        int amount = 0;
        foreach (Chip chip in chips)
        {
            amount += (int)chip;
        }
        return amount;
    }
}
