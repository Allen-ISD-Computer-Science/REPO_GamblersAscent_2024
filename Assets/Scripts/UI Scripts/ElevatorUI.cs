using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElevatorUI : MonoBehaviour
{

    private GameObject cameraObject;
    private GameObject background;
    private GameObject backgroundBarriers;
    private GameObject player;

    private float cameraX, cameraY;
    private bool elevatorUIActive;

    // The 5 buttons for the elevator UI, with floors 0-5 respectively
    //   private GameObject button0, button1, button2, button3, button4, button5;
    private GameObject[] buttons = new GameObject[6];

    // Start is called before the first frame update
    void Start()
    {
        // Get the player object
        player = GameObject.Find("Player");

        // Get the camera's position and set the elevator UI to it
        cameraObject = GameObject.FindWithTag("MainCamera");

        // Get the background of the 5 floors
        background = GameObject.Find("Backgrounds");

        // Get the barriers of the 5 floors
        backgroundBarriers = GameObject.Find("Grid");

        // Get the buttons for the elevator UI
        buttons = new GameObject[]
        {
            transform.Find("ElevatorButton-0").gameObject,
            transform.Find("ElevatorButton-1").gameObject,
            transform.Find("ElevatorButton-2").gameObject,
            transform.Find("ElevatorButton-3").gameObject,
            transform.Find("ElevatorButton-4").gameObject,
            transform.Find("ElevatorButton-5").gameObject
        };
    }

    // Update is called once per frame
    void Update()
    {

        cameraX = cameraObject.transform.position.x;
        cameraY = cameraObject.transform.position.y;
        // Set the UI to the camera's position to follow it
        transform.position = new Vector3(cameraX, cameraY, 0);
        // Moves the UI similarly to the camera
        /*
        Vector3 targetPos = new Vector3(cameraX, cameraY, 0);
        Vector3 velocity = (targetPos - transform.position) * player.GetComponent<IsometricPlayerMovementController>().movementSpeed;
        transform.position = Vector3.SmoothDamp(transform.position, targetPos, ref velocity, 1.0f, Time.deltaTime);
        */
        if (elevatorUIActive)
        {
            // Check for mouse input
            if (Input.GetMouseButtonDown(0))
            {
                // Get the mouse position
                Vector3 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);

                bool buttonClicked = false; // Flag to check if any button was clicked

                for (int i = 0; i < 6; i++)
                {
                    // Check if the mouse is over the button
                    if (buttons[i].GetComponent<Collider2D>().OverlapPoint(mousePos))
                    {
                        // Enables the child floor's background
                        background.transform.GetChild(i).gameObject.SetActive(true);

                        // Enables the child floor's barriers
                        backgroundBarriers.transform.GetChild(i).gameObject.SetActive(true);

                        // Set the player's current floor to the button's floor
                        player.GetComponent<PlayerInformation>().currentFloor = i;

                        // Sets the player Layer to the current floor
                        player.layer = i + 6;
                        player.transform.GetChild(0).gameObject.layer = i + 6;


                        buttonClicked = true; // Set the flag to true
                    }
                    else
                    {
                        // Disables the child floor's background
                        background.transform.GetChild(i).gameObject.SetActive(false);

                        // Disables the child floor's barriers
                        backgroundBarriers.transform.GetChild(i).gameObject.SetActive(false);
                    }
                }

                // If no button was clicked, display the player's current floor
                if (!buttonClicked)
                {
                    int currentFloor = player.GetComponent<PlayerInformation>().currentFloor;
                    background.transform.GetChild(currentFloor).gameObject.SetActive(true);
                    backgroundBarriers.transform.GetChild(currentFloor).gameObject.SetActive(true);
                }
            }
        }
    }

    void OnEnable()
    {
        OpenElevatorUI();
    }

    void OnDisable()
    {
        CloseElevatorUI();
    }

    void OpenElevatorUI()
    {
        // Starts checking elevator UI mouse inputs
        elevatorUIActive = true;
    }

    void CloseElevatorUI()
    {
        // Stops the UI mouse checks
        elevatorUIActive = false;
        
        // Close the UI
        gameObject.SetActive(false);
    }


}
