using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Hand 
{
    public List<Card> Cards { get; } = new List<Card>();
    public bool IsSoft { get; private set; }

    public void AddCard(Card card)
    {
        Cards.Add(card);
    }

    public int HandValue()
    {
        int result = 0;
        IsSoft = false;
        int numAces = 0;

        foreach (var card in Cards)
        {
            int cardValue = card.GetValue();

            if (card.Rank == Rank.Ace)
            {
                numAces++;
            }
            else
            {
                result += cardValue;
            }
        }

        while (numAces > 0)
        {
            if (result + 11 <= 21)
            {
                result += 11;
                numAces--;
            }
            else
            {
                result += 1;
                numAces--;
                IsSoft = true;
            }
        }

        return result;
    }

    public void ClearHand()
    {
        Cards.Clear();
    }

    public bool IsPair()
    {
        return Cards[0].Rank == Cards[1].Rank;
    }

    public bool IsBust()
    {
        return HandValue() > 21;
    }

    public bool IsBlackjack()
    {
        return HandValue() == 21 || (Cards.Count >= 5 && HandValue() < 22);
    }

    public void UpdateSoftStatus()
    {
        IsSoft = false;
        foreach (var card in Cards)
        {
            if (card.Rank == Rank.Ace && HandValue() <= 11)
            {
                IsSoft = true;
                break;
            }
        }
    }

    public override string ToString()
    {
        return string.Join("\n", Cards);
    }

    internal int CardsInHand()
    {
        return Cards.Count;
    }
}

