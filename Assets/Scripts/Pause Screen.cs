using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PauseScreen : MonoBehaviour
{
    // Start is called before the first frame update
    private GameObject pauseScreenUI;
    private GameObject mainMenuUI;
    void Start()
    {
        mainMenuUI = GameObject.Find("Main Menu UI");
        pauseScreenUI = transform.GetChild(0).gameObject;
        pauseScreenUI.GetComponent<PauseScreenUI>().mainMenuUI = mainMenuUI;

    }

    // Update is called once per frame
    void Update()
    {
        if (!mainMenuUI.activeSelf && Input.GetKeyDown(KeyCode.Escape))
        {
            if (pauseScreenUI.activeSelf)
            {
                pauseScreenUI.SetActive(false);
                Time.timeScale = 1;
            }
            else
            {
                pauseScreenUI.SetActive(true);
                Time.timeScale = 0;
            }
        }
    }
}
