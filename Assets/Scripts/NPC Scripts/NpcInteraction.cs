using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NpcInteraction : MonoBehaviour
{
    private bool triggerActive;
    public bool waitForDisable;

    public GameObject interaction;
    // Start is called before the first frame update
    void Start()
    {
        triggerActive = false;

    }

    public void OnTriggerEnter2D(Collider2D other)
    {
        if (other.CompareTag("Player") && interaction != null)
        {
            triggerActive = true;
        }
    }

    public void OnTriggerExit2D(Collider2D other)
    {
        if (other.CompareTag("Player"))
        {
            triggerActive = false;
            if (waitForDisable && interaction != null)
            {
                try
                {
                    if (interaction.GetComponent<NPCHandler>().playingBlackjack)
                    {
                        // Won't close the interaction if the NPC is playing blackjack, as the player will need to interact with the NPC to play
                    }
                    else
                    {
                        CloseNPCAction();
                    }

                }
                catch
                {
                    CloseNPCAction();
                }
            }
        }
    }

    private void Update()
    {
        if (triggerActive && Input.GetKeyDown(KeyCode.E))
        {
            TriggerNPCAction();
        }
    }

    // Opens the elevator UI
    public void TriggerNPCAction()
    {
        interaction.SetActive(true);
    }

    public void CloseNPCAction()
    {
        
        interaction.SetActive(false);
    }

}
