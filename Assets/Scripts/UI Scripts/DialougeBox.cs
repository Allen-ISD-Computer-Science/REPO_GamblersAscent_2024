using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DialougeBox : MonoBehaviour
{
    private GameObject cameraObject;
    private GameObject player;

    private GameObject dialougeBoxBottom;
    private GameObject dialougeBoxTop;
    private GameObject dialougeBoxText;
    private GameObject dialougeBoxName;

    private Coroutine revealCoroutine;

    private readonly int textShiftX = -10;
    private readonly int textShiftY = -140;

    public float delay = 0.1f; // delay between each character
    public Color visibleColor = Color.white; // color for visible text
    public Color hiddenColor = Color.clear; // color for hidden text

    private TMPro.TextMeshPro textComponent;// The text component to render the dialouge


    public string dialougeLine; // The line of text to display
    public bool isTyping = false; // Is the text being typed out? Or is it finished?

    private void Awake()
    {
        cameraObject = GameObject.FindWithTag("MainCamera");  // Get the camera object
        // Get the player object
        player = GameObject.Find("Player");

        // Get the dialouge box objects
        dialougeBoxTop = transform.GetChild(0).gameObject;
        dialougeBoxBottom = transform.GetChild(1).gameObject;
        dialougeBoxText = transform.GetChild(2).gameObject;
        dialougeBoxName = transform.GetChild(3).gameObject;
        // Get the text component
        textComponent = dialougeBoxText.GetComponent<TMPro.TextMeshPro>();
    }
    public void DisplayText(string text)
    {
        if (revealCoroutine != null)
        {
            StopCoroutine(revealCoroutine);
        }
        revealCoroutine = StartCoroutine(RevealCharacters(text));
    }

    public void StopRevealCharacters()
    {
        if (revealCoroutine != null)
        {
            StopCoroutine(revealCoroutine);
        }
    }

    public void DisplayAllText(string text)
    {
        textComponent.text = text;
        textComponent.ForceMeshUpdate(); // update the mesh and the characters

        TMPro.TMP_TextInfo textInfo = textComponent.textInfo;
        int vertexCount = textInfo.meshInfo[0].vertices.Length / 4;

        for (int i = 0; i < vertexCount; i++)
        {
            // make all characters visible
            textInfo.meshInfo[0].colors32[i * 4] = visibleColor;
            textInfo.meshInfo[0].colors32[i * 4 + 1] = visibleColor;
            textInfo.meshInfo[0].colors32[i * 4 + 2] = visibleColor;
            textInfo.meshInfo[0].colors32[i * 4 + 3] = visibleColor;
        }

        // Update the mesh with the new colors
        textComponent.UpdateVertexData(TMPro.TMP_VertexDataUpdateFlags.Colors32);

        isTyping = false;
    }
    /*
       A coroutine is a function that can pause and resume its execution at any point, allowing for complex behavior over several frames. In this 
       context it is used to display the text one character at a time, with a delay between each character.
    */

    private IEnumerator RevealCharacters(string text) // An IEnumerator is a coroutine. It is used to display the text one character at a time, with a delay between each character.
    {
        textComponent.text = text; // set the text
        textComponent.ForceMeshUpdate(); // update the mesh and the characters

        TMPro.TMP_TextInfo textInfo = textComponent.textInfo; // Get the text info. Has information about the text, like character count, etc.

    //  int characterCount = textInfo.characterCount; Unused for now
        int vertexCount = textInfo.meshInfo[0].vertices.Length / 4;

        for (int i = 0; i < vertexCount; i++)
        {
            // initially hide all characters
            textInfo.meshInfo[0].colors32[i * 4] = hiddenColor;
            textInfo.meshInfo[0].colors32[i * 4 + 1] = hiddenColor;
            textInfo.meshInfo[0].colors32[i * 4 + 2] = hiddenColor;
            textInfo.meshInfo[0].colors32[i * 4 + 3] = hiddenColor;
        }

        // Update the mesh with the new colors
        textComponent.UpdateVertexData(TMPro.TMP_VertexDataUpdateFlags.Colors32);

        for (int i = 0; i < vertexCount; i++)
        {
            // reveal each character one by one
            textInfo.meshInfo[0].colors32[i * 4] = visibleColor;
            textInfo.meshInfo[0].colors32[i * 4 + 1] = visibleColor;
            textInfo.meshInfo[0].colors32[i * 4 + 2] = visibleColor;
            textInfo.meshInfo[0].colors32[i * 4 + 3] = visibleColor;

            // Update the mesh with the new colors
            textComponent.UpdateVertexData(TMPro.TMP_VertexDataUpdateFlags.Colors32);

            yield return new WaitForSeconds(delay);
        }
        isTyping = false;
    }
    void Update()
    {
        // Set the position of the dialouge box to the camera position
        dialougeBoxTop.transform.position = new Vector3(cameraObject.transform.position.x, cameraObject.transform.position.y, 0);
        dialougeBoxBottom.transform.position = new Vector3(cameraObject.transform.position.x, cameraObject.transform.position.y, 0);
        dialougeBoxText.transform.position = new Vector3(cameraObject.transform.position.x + textShiftX, cameraObject.transform.position.y + textShiftY, 0);
        dialougeBoxName.transform.position = new Vector3(cameraObject.transform.position.x + textShiftX - 210, cameraObject.transform.position.y + textShiftY + 70, 0);
        
    }

    private void OnEnable()
    {
        // Set the starting position of the dialouge box to the camera position
        dialougeBoxTop.transform.position = new Vector3(cameraObject.transform.position.x, cameraObject.transform.position.y, 0);
        dialougeBoxBottom.transform.position = new Vector3(cameraObject.transform.position.x, cameraObject.transform.position.y, 0);

        dialougeBoxText.transform.position = new Vector3(cameraObject.transform.position.x + textShiftX, cameraObject.transform.position.y + textShiftY, 0);
        dialougeBoxName.transform.position = new Vector3(cameraObject.transform.position.x + textShiftX - 210, cameraObject.transform.position.y + textShiftY + 70, 0);

    }
    
    private void OnDisable()
    {
        isTyping = false;

    }
}
