using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class NPCHandler : MonoBehaviour
{

    public string[] dialouge;
    public int dialougeIndex;
    public GameObject dialougeBoxUI;
    public GameObject ShopUI;
    public GameObject blackjack;

    // Shop variables
    public bool openShop;

    // Blackjack variables, optional because not all NPCs will play blackjack
    public bool gambler;
    public bool playingBlackjack;


    void Awake()
    {
        dialougeIndex = 0;
    }

    private void OnEnable()
    {

        if (dialougeBoxUI != null)
        {
            dialougeBoxUI.SetActive(true);
            dialougeBoxUI.GetComponent<DialougeBox>().isTyping = true;
            dialougeBoxUI.transform.GetChild(2).GetComponent<TextMeshPro>().text = dialouge[dialougeIndex];
            dialougeBoxUI.GetComponent<DialougeBox>().DisplayText(dialouge[dialougeIndex]);
            dialougeBoxUI.transform.GetChild(3).GetComponent<TextMeshPro>().text = gameObject.name;
        }
        else if (blackjack != null)
        {
            blackjack.SetActive(true);
        }
    }

    private void OnDisable()
    {
        if (dialougeBoxUI != null)
        {
            dialougeBoxUI.SetActive(false);
            dialougeIndex = 0;
        }
        if (ShopUI != null)
        {
            ShopUI.SetActive(false);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (dialougeBoxUI != null && dialougeBoxUI.activeSelf)
        {
            DialougeBox dialougeBox = dialougeBoxUI.GetComponent<DialougeBox>();
            bool isTyping = dialougeBox.isTyping;

            if (Input.GetKeyDown(KeyCode.E))
            {
                if (isTyping)
                {
                    dialougeBox.StopRevealCharacters();
                    dialougeBox.DisplayAllText(dialouge[dialougeIndex]);
                }
                else
                {
                    if (dialougeIndex < dialouge.Length - 1)
                    {
                        dialougeIndex++;
                        dialougeBox.isTyping = true;
                        dialougeBox.DisplayText(dialouge[dialougeIndex]);
                    }
                    else
                    {
                        if (gambler)
                        {
                            gameObject.tag = "Opponent";
                            
                            dialougeBoxUI.SetActive(false);
                            dialougeIndex = 0;
                            dialougeBox.isTyping = false;
                            playingBlackjack = true;
                            blackjack.SetActive(true);
                        }
                        else if (openShop)
                        {
                            dialougeBoxUI.SetActive(false);
                            dialougeIndex = 0;
                            dialougeBox.isTyping = false;
                            ShopUI.SetActive(true);
                        }
                        else
                        {
                            dialougeBoxUI.SetActive(false);
                            dialougeIndex = 0;
                            dialougeBox.isTyping = false;
                            gameObject.SetActive(false);
                        }
                    }
                }
            }
        }
    }
}
