using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlackjackRender : BlackjackBackend
{
    private GameObject cameraObject;
    int cameraX, cameraY;


    // Start is called before the first frame update
    void OnEnable()
    {
        cameraObject = GameObject.Find("Main Camera");

    }

    // Update is called once per frame
    void Update()
    {
        cameraX = (int)cameraObject.transform.position.x;
        cameraY = (int)cameraObject.transform.position.y;
        transform.position = new Vector3(cameraX, cameraY, 0);
    }
}
