using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainMenuUI : MonoBehaviour
{
    private GameObject cameraObject;

    private GameObject player;
    [SerializeField]
    private GameObject blackjack;

    private GameObject[] buttons;
    int cameraX, cameraY;

    void Start() {
        player = GameObject.Find("Player");

    }
    void OnEnable() {
        cameraObject = GameObject.Find("Main Camera");
        cameraX = (int)cameraObject.transform.position.x;
        cameraY = (int)cameraObject.transform.position.y;
        transform.position = new Vector3(cameraX, cameraY, 0);
        buttons = new GameObject[]
        {
            transform.GetChild(0).gameObject, // New Game
            transform.GetChild(1).gameObject, // Continue
            transform.GetChild(2).gameObject, // Settings
            transform.GetChild(3).gameObject, // Credits
            transform.GetChild(4).gameObject // Quit
        };
        Time.timeScale = 0;
    

    }

    // Update is called once per frame
    void Update()
    {
        cameraX = (int)cameraObject.transform.position.x;
        cameraY = (int)cameraObject.transform.position.y;
        transform.position = new Vector3(cameraX, cameraY, 0);
        if (Input.GetMouseButtonDown(0))
        {
            Vector3 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            RaycastHit2D hit = Physics2D.Raycast(mousePos, Vector2.zero);
            if (hit.collider != null)
            {
                if (hit.collider.gameObject == buttons[0])
                {
                    Debug.Log("New Game");
                    Time.timeScale = 1;
                    // Starts a new game. Sets the money to 1000, max floor to 0, and resets the player's position.
                    player.transform.position = new Vector3(517, -264, 0);
                    player.GetComponent<PlayerInformation>().money = 1000;
                    player.GetComponent<PlayerInformation>().maxFloor = 0;
                    player.GetComponent<PlayerInformation>().currentFloor = 0;
                    // Sets the player's layer to 6 (Floor 0)
                    player.layer = 6;
                    player.transform.GetChild(0).gameObject.layer = 6;
                    player.SetActive(true);

                    // Disable all floors
                    for (int i = 0; i < 5; i++)
                    {
                        GameObject.Find("Backgrounds").transform.GetChild(i).gameObject.SetActive(false);
                    }
                    for (int i = 0; i < 5; i++)
                    {
                        GameObject.Find("Grid").transform.GetChild(i).gameObject.SetActive(false);
                    }

                    // Sets the background to the first floor
                    GameObject.Find("Backgrounds").transform.GetChild(0).gameObject.SetActive(true);

                    // Sets the Grid to the first floor
                    GameObject.Find("Grid").transform.GetChild(0).gameObject.SetActive(true);

                    // Ends any blackjack games that are running.
                    blackjack.SetActive(false);
                    
                    gameObject.SetActive(false);
                }
                else if (hit.collider.gameObject == buttons[1]) // Continue
                {
                    Time.timeScale = 1;
                    if (!player.activeSelf)
                    {
                        Debug.Log("Player not enabled, turning it on if blackjack isn't running");
                        try
                        {
                            blackjack = GameObject.Find("Blackjack").gameObject;
                        } 
                        catch 
                        { 
                           Debug.Log("No Blackjack, enabling player");
                           player.SetActive(true);
                        }
                        
                    }
                    
                    gameObject.SetActive(false);
                }
                else if (hit.collider.gameObject == buttons[2]) // Settings
                {
                    Debug.Log("Settings");
                }
                else if (hit.collider.gameObject == buttons[3]) // Credits
                {
                    Debug.Log("Credits");
                }
                else if (hit.collider.gameObject == buttons[4]) // Quit
                {
                    Application.Quit();
                    Debug.Log("Quit");
                }
            }
        }
    }
}
