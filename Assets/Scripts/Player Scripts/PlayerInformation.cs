using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum Ability
{
    RemnantFlame,
    RoyaleExecution,
    AbsoluteZero,
    None,
}

public class PlayerInformation : MonoBehaviour
{
    // General variables
    public string playerName;
    public int currentFloor;
    public int maxFloor;
    public int money;

    // Blackjack variables
    public Ability ability;
    public Hand hand;
    public bool isDealer;
    public List<Chip> currentChips;

    // Blackjack variables
    // WIP


    // Start is called before the first frame update
    void Start()
    {
        currentFloor = 0;
        maxFloor = 0;
        ability = Ability.RemnantFlame;

        Debug.Log("Player Information: " + currentFloor + " " + maxFloor + " " + money + " " + ability);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
