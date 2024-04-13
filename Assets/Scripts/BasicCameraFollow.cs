using UnityEngine;
using System.Collections;

public class BasicCameraFollow : MonoBehaviour 
{

	private Vector3 startingPosition;
	public Transform followTarget;
	private Vector3 targetPos;
	public float moveSpeed;
	private float minX, maxX, minY, maxY;
    
	void Start()
	{
		startingPosition = transform.position;
		Debug.Log("Camera Pixel width: " + Camera.main.pixelWidth);
        Debug.Log("Camera Pixel height: " + Camera.main.pixelHeight);
        minX = -320f;
		maxX = 315f;
		minY = -175f;
		maxY = 180f;
    }

    void Update () 
	{
		if(followTarget != null)
		{
			targetPos = new Vector3(followTarget.position.x, followTarget.position.y, transform.position.z);
			Vector3 velocity = (targetPos - transform.position) * moveSpeed;
			transform.position = Vector3.SmoothDamp (transform.position, targetPos, ref velocity, 1.0f, Time.deltaTime);
			transform.position = new Vector3(Mathf.Clamp(transform.position.x, minX, maxX), Mathf.Clamp(transform.position.y, minY, maxY), transform.position.z);
		}
	}
}

