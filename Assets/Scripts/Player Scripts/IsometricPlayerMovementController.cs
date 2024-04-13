using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IsometricPlayerMovementController : MonoBehaviour
{

    public float movementSpeed = 1f;
    IsometricCharacterRenderer isoRenderer;
    Rigidbody2D rbody;
    CircleCollider2D circleCollider;

    private void Awake()
    {
       rbody = GetComponent<Rigidbody2D>();
       isoRenderer = GetComponentInChildren<IsometricCharacterRenderer>();
       circleCollider = GetComponentInChildren<CircleCollider2D>();
    }


    // Update is called once per frame
    void FixedUpdate()
    {
        // Debug.Log("FixedUpdate time: " + Time.time);
        Vector2 currentPos = rbody.position;
        float horizontalInput = Input.GetAxisRaw("Horizontal");
        float verticalInput = Input.GetAxisRaw("Vertical");
        // If the player is moving in 2 directions at once, we make sure they follow the grid by halving the y movement
        if (horizontalInput != 0 && verticalInput != 0)
        {
            verticalInput /= 2;
        }
        Vector2 inputVector = new Vector2(horizontalInput, verticalInput);
        inputVector = Vector2.ClampMagnitude(inputVector, 1);
        Vector2 movement = inputVector * movementSpeed;
        Vector2 newPos = currentPos + movement * Time.fixedDeltaTime;
        isoRenderer.SetDirection(movement);
        rbody.MovePosition(newPos);
       // circleCollider.offset = new Vector2(movement.x, movement.y);
    }
}
