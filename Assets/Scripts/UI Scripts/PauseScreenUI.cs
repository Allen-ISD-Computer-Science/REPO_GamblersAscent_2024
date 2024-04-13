using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PauseScreenUI : MonoBehaviour
{
    private GameObject cameraObject;
    public GameObject mainMenuUI;
    int cameraX, cameraY;

    
    void OnEnable() {

    cameraObject = GameObject.FindWithTag("MainCamera");

    // Prevent all game objects from moving when the pause screen is active. 
    cameraX = (int)cameraObject.transform.position.x;
    cameraY = (int)cameraObject.transform.position.y;
    transform.position = new Vector3(cameraX, cameraY, 0);

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if (gameObject.activeSelf)
            {
                gameObject.SetActive(false);
                Time.timeScale = 1;
            }
            else
            {
                gameObject.SetActive(true);
                Time.timeScale = 0;
            }
        }

        if (Input.GetMouseButtonDown(0))
        {
            Vector3 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            RaycastHit2D hit = Physics2D.Raycast(mousePos, Vector2.zero);
            if (hit.collider != null)
            {
                if (hit.collider.gameObject.name == "Resume")
                {
                    gameObject.SetActive(false);
                    Time.timeScale = 1;
                }
                else if (hit.collider.gameObject.name == "Options")
                {
                    Debug.Log("Options");
                }
                else if (hit.collider.gameObject.name == "Exit")
                {
                    try {
                        GameObject blackjack = GameObject.Find("Blackjack");
                        blackjack.SetActive(false);
                    } 
                    catch { Debug.Log("No Blackjack"); }

                    try {
                        GameObject player = GameObject.Find("Player");
                        player.SetActive(false);
                    }
                    catch { Debug.Log("No Player"); }

                    mainMenuUI.SetActive(true);

                    gameObject.SetActive(false);
                    Debug.Log("Exit");
                }
            }
        }
    }
}
