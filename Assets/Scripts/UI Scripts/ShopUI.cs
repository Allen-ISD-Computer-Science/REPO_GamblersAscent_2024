using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class ShopUI : MonoBehaviour
{
    // Start is called before the first frame update

    private GameObject cameraObject;
    private GameObject player;

    private float cameraX, cameraY;
    private bool shopUIActive;

    private GameObject[] elevatorKeys = new GameObject[5];

    private GameObject currentKey;
    int keyPrice;

    void Start()
    {
        // Get the player object
        player = GameObject.Find("Player");

        // Get the camera's position and set the elevator UI to it
        cameraObject = GameObject.FindWithTag("MainCamera");

        elevatorKeys = new GameObject[]
        {
            transform.GetChild(0).transform.GetChild(0).gameObject,
            transform.GetChild(0).transform.GetChild(1).gameObject,
            transform.GetChild(0).transform.GetChild(2).gameObject,
            transform.GetChild(0).transform.GetChild(3).gameObject,
            transform.GetChild(0).transform.GetChild(4).gameObject,
        };
        
    }

    // Update is called once per frame
    void Update()
    {
        cameraX = cameraObject.transform.position.x;
        cameraY = cameraObject.transform.position.y;
        // Set the UI to the camera's position to follow it
        transform.position = new Vector3(cameraX, cameraY, 0);
        
        if (player.GetComponent<PlayerInformation>().maxFloor == 0)
        {
            currentKey = elevatorKeys[0];
        }
        else
        {
            currentKey = elevatorKeys[player.GetComponent<PlayerInformation>().maxFloor - 1];
        }
        currentKey.SetActive(true);
        
        if (shopUIActive)
        {
            // Checks if the player can afford the key ( doesn't include floor 1 key since it is free)
            if (int.TryParse(currentKey.transform.GetChild(2).GetChild(0).GetComponent<TMPro.TextMeshPro>().text, out keyPrice))
            {
                if (player.GetComponent<PlayerInformation>().money < keyPrice)
                {
                    currentKey.GetComponent<BoxCollider2D>().enabled = false;
                    currentKey.transform.GetChild(3).gameObject.SetActive(false);
                    currentKey.transform.GetChild(4).gameObject.SetActive(true);
                }
                else
                {
                    currentKey.GetComponent<BoxCollider2D>().enabled = true;
                    currentKey.transform.GetChild(3).gameObject.SetActive(true);
                    currentKey.transform.GetChild(4).gameObject.SetActive(false);
                }
            }
            

            // Check for mouse input
            if (Input.GetMouseButtonDown(0))
            {
                // Get the mouse position
                Vector3 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);

                if (currentKey.GetComponent<Collider2D>().OverlapPoint(mousePos))
                {
                    Debug.Log("Key clicked");
                    // Sets the new max floor and subtracts the price from the player's money
                    player.GetComponent<PlayerInformation>().maxFloor++;
                    player.GetComponent<PlayerInformation>().money -= keyPrice;
                    currentKey.SetActive(false);
                    currentKey = elevatorKeys[player.GetComponent<PlayerInformation>().maxFloor - 1];
                    currentKey.SetActive(true);
                }
                
            }
        }
    }
    void OnEnable()
    {
        OpenShopUI();
    }

    void OnDisable()
    {
        CloseShopUI();
    }

    void OpenShopUI()
    {
        // Starts checking shop UI mouse inputs
        shopUIActive = true;
    }

    void CloseShopUI()
    {
        // Stops the UI mouse checks
        shopUIActive = false;

        // Close the UI
        gameObject.SetActive(false);
    }
}
