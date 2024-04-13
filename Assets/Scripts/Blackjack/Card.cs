using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum Suit
{
    Spades = 1,
    Clubs = 2,
    Diamonds = 3,
    Hearts = 4
}

public enum Rank
{
    Ace = 1,
    Two = 2,
    Three = 3,
    Four = 4,
    Five = 5,
    Six = 6,
    Seven = 7,
    Eight = 8,
    Nine = 9,
    Ten = 10,
    Jack = 11,
    Queen = 12,
    King = 13
}

public class Card 
{
    public Suit Suit { get; }
    public Rank Rank { get; }
    
    // Card's sprite image gotten from the suit and rank using the image's name off the template (e.g. "Two_Of_Clubs" and "Ace_Of_Spades" ) at the path "Assets/Tilemaps/Blackjack/Playing_Cards/"
    public Sprite CardSprite => Resources.Load<Sprite>($"Blackjack/Playing_Cards/{Rank}_Of_{Suit}");
    public Card(Suit suit, Rank rank)
    {
        Suit = suit;
        Rank = rank;
}

    public int GetValue()
    {
        switch (Rank)
        {
            case Rank.Ace:
                return 11;
            case Rank.Two:
                return 2;
            case Rank.Three:
                return 3;
            case Rank.Four:
                return 4;
            case Rank.Five:
                return 5;
            case Rank.Six:
                return 6;
            case Rank.Seven:
                return 7;
            case Rank.Eight:
                return 8;
            case Rank.Nine:
                return 9;
            case Rank.Ten:
            case Rank.Jack:
            case Rank.Queen:
            case Rank.King:
            default:
                return 10;
        }
    }

    public override string ToString()
    {
        return $"The {GetValue()} of {Suit}";
    }
}
