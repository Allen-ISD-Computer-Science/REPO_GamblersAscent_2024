using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum Action
{
    Hit = 'H',
    Stand = 'S',
    Double = 'D',
    Split = 'C',
    Insurance = 'I',
    Bust = 84,
}
public class BlackjackAI
{
    private const int baseValue = 9;
    private int cardInfluence;
    private int handInfluence;
    private System.Random random = new();
    private int rngValue;
    private int luckRNG;
    private int maxHand;

    private readonly int aggro;
    private readonly int defense;
    private readonly int risk;
    private readonly int luck;
    private Ability ability;

    public BlackjackAI(int aggro, int defense, int risk, int luck, Ability ability = Ability.None)
    {
        this.aggro = aggro;
        this.defense = defense;
        this.risk = risk;
        this.luck = luck;
        this.ability = ability;

        rngValue = random.Next(-1, 2);
        luckRNG = random.Next(1, 101);
    }

    public Action DetermineAction(Hand hand, Hand playerHand)
    {
        handInfluence = (playerHand.HandValue() > 17) ? 1 : -1;
        maxHand = baseValue + defense + handInfluence + rngValue;

        if (hand.HandValue() > playerHand.HandValue() || hand.HandValue() >= 21)
        {
            return Action.Stand;
        }

        if (hand.HandValue() == playerHand.HandValue() && defense >= 5)
        {
            return Action.Stand;
        }
        else
        {
            return Action.Hit;
        }
    }
}

