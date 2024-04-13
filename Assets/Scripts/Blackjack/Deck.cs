using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class Deck 
{
    private List<Card> cards = new();
    public List<Card> discardPile = new();
    private readonly System.Random random = new();

    public Deck()
    {
        for (int s = 1; s <= 4; s++)
        {
            for (int r = 1; r <= 13; r++)
            {
                cards.Add(new Card((Suit)s, (Rank)r));
            }
        }
    }

    public void Shuffle()
    {
        int n = cards.Count;
        for (int i = n - 1; i > 0; i--)
        {
            int j = random.Next(i + 1);
            (cards[j], cards[i]) = (cards[i], cards[j]);
        }
    }

    public void Reshuffle()
    {
        if (cards.Count == 0 && discardPile.Count == 0)
        {
            for (int s = 1; s <= 4; s++)
            {
                for (int r = 1; r <= 13; r++)
                {
                    cards.Add(new Card((Suit)s, (Rank)r));
                }
            }
        }
        else
        {
            cards.AddRange(discardPile);
            discardPile.Clear();
        }

        Shuffle();
    }

    public void Discard(Card card)
    {
        discardPile.Add(card);
    }

    public void Discard(List<Card> cards)
    {
        discardPile.AddRange(cards);
    }

    public Card PeekTop()
    {
        return cards[0];
    }

    public void ReplaceTop(Card card)
    {
        Card topCard = cards[0];
        discardPile.Add(topCard);
        cards[0] = card;
    }

    public Card DealCard()
    {
        if (cards.Count == 0)
        {
            Reshuffle();
        }

        Card topCard = cards[0];
        cards.RemoveAt(0);
        discardPile.Add(topCard);
        return topCard;
    }

    public int CardsLeft()
    {
        return cards.Count;
    }

    public Deck MergeDecks(Deck deck)
    {
        cards.AddRange(deck.cards);
        return this;
    }

    public void ClearDeck()
    {
        cards.Clear();
        discardPile.Clear();
    }
}
