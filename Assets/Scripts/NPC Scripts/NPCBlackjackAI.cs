using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NPCBlackjackAI : MonoBehaviour
{
    public int money;
    public Ability ability;
    public Hand hand; 
    public bool isDealer;
    public List<Chip> currentChips;

    [SerializeField]
    private int aggro;
    [SerializeField]
    private int defense;
    [SerializeField]
    private int risk;
    [SerializeField]
    private int luck;
    private readonly int baseValue = 9;
    private int rngValue;
    private int luckRNG;

    // Start is called before the first frame update
    void Start()
    {
        rngValue = Random.Range(-1, 1);
        luckRNG = Random.Range(0, 100);
    }

    public Action DetermineAction(Hand playerHand, Card dealerCard) {
        int cardInfluence = (dealerCard.GetValue() > 5) ? 1 : -1;
        int maxHand = baseValue + aggro + cardInfluence + rngValue;

        // Preconditions
        if (playerHand.HandValue() >= 21) { return Action.Stand; }
        if (playerHand.HandValue() < dealerCard.GetValue()) { return Action.Hit; }

        // Double & Split Calculation
        if (playerHand.CardsInHand() == 2)
        {
            // Splitting
            /* if (hand.cards[0].get_value() == hand.cards[1].get_value())
            {

                 return Action::Split;
            }
            */
            // Doubling
            if (playerHand.HandValue() == 11)
            {
                return Action.Double;
            }
            // Soft Hand 
            else if (hand.IsSoft && playerHand.HandValue() >= 16 && playerHand.HandValue() <= 18)
            {
                return Action.Double;
            }

        }

        // If the hand value is greater than the maxHand, the AI will stand
        if (playerHand.HandValue() >= maxHand)
        {
            return Action.Stand;
        }
        else
        {
            return Action.Hit;
        }

    }

    public Action DetermineAction(Hand dealerHand, Hand playerHand)
    {
        int handInfluence = (playerHand.HandValue() > 17) ? 1 : -1;
        int maxHand = baseValue + defense + rngValue + handInfluence;

        // Preconditions
        if (dealerHand.HandValue() >= 17) { return Action.Stand; }
        if (dealerHand.HandValue() > playerHand.HandValue()) { return Action.Stand; }

        // If the hand value is greater than the maxHand, the AI will stand
        if (dealerHand.HandValue() >= maxHand)
        {
            return Action.Stand;
        }
        else
        {
            return Action.Hit;
        }
    }

    public int DetermineBet()
    {
        return (int)((float)money / 4f * (1 + (float)risk / 10f)); // Needs to be an integer value because the player can't bet a fraction of a chip
    }

    public bool DetermineLuck()
    {
	    // The luck attribute represents the % chance of the ai getting +-1 to the card value needed for a blackjack. If hand value is too small, then the ai will draw as normal
	    return (luckRNG <= luck * 10);
    }
}
