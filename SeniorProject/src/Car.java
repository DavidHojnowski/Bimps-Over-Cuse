
public class Car {
	/*
	 * A model for the actual car that we will be using
	 * Holds the values for the state of the car 
	 */
	
	private int speed; //int value for the speed of the car
	
	private boolean fwd; //if true the car is moving forward
	private boolean bckwd; //if true the car is moving backwards
	//fwd and backward should not both be true 
	private boolean left; //if true car is going to the left
	private boolean right;//if true the car is moving right
	//left and right should not both be true
	private boolean moving;//if true if the car is moving else the car is stoped
	//This boolean tells us if it's beining manually controlled or atomtic
	private boolean manual; //if true then we are in manual mode
	
	public Car(){
		//speed = ?? to do find some intial value for speed
		fwd = false;
		bckwd = false;
		left = false;
		right = false;
		moving = false;	
		manual = true;
	}
	
	//Pre:Neeed to update the state of the car to refelect breaking and not moving
	//Post:The state of the car has been updated to be no longer moving 
	//Call this method when the car is breaking or not moving
	public void brake(){
		fwd = false;
		bckwd = false;
		left = false;
		right = false;
		moving = false;
	}
	//Pre: Need to set the speed of the car to some known amount
	//Post: The speed has been set and uodated for the state of the car
	//Call this method to increase the speed by some known amount
	public void setSpeed(int speed){
		this.speed = speed;
	}
	//Pre:Need to get the curent speed of the sate of the car
	//Post: Returns the value of speed as an int
	//Get the speed of the car currently
	public int getSpeed(){
		return speed;
	}
	//Pre:Neeed to increase the speed of the car by some increment amount
	//Post: the speed has been increased by some set amount
	//when we just want to increase the speed by some increment
	//May not need this one (But maybe we will)
	public void speedUp(){
		speed++; //may increase this to increment by some other value
	}
	//Pre: Need to decrease the speed of the sate of the car by an increment amount	
	//Post: The speed has been decreased by some set amount
	//When we dont know the exact new speed we want so we decrerase it by some increment
	public void speedDown(){
		speed--;
	}
	
	//Pre: Need to update the moving state of the car based on the ohter states it has
	//Post: The moving state has now been updated to true or false
	//Want to check based on the currnt state of the car if it's moving 
	private void updateMoving(){
		moving = left || right || fwd || bckwd; //if any of them are true then were moving in some direction
		//if all are false then we are not moving 
	}
	
	//Pre:Need to update the state of the car to move to the left
	//Post: The state of the car has been updated and is moving to the left
	//call this method when the car is now going left
	public void goLeft(){
		//set all other direstions to false
		right = false;//we want to make sure right is false when left is true
		fwd = false;
		bckwd = false;
		left = true;
		updateMoving();
	}
	//Pre:Need tot update the state to stop moving to the left
	//Post:The state of the car is no longer going to the left
	//call this method when we no longer are heading left
	public void stopLeft(){
		left = false;
		//want to update moving now to see if it is stoped moving
		updateMoving();
	}
	//Pre: Need to know if the car is moving to the left
	//Post: Retruns true if the car is moving to the left false if it is not 
	//want to know if were heading left, if going left then retunrs true else false
	public boolean goingLeft(){
		return left;
	}
	//Pre:Need to set the sate of the car to be moving to the right
	//Post:The state of the car has been updated and is now moving to the right
	//When this method is called the car is going right
	public void goRight(){
		left = false;//can not begoing both left and right at the same time
		fwd = false;
		bckwd = false;
		right = true;
		updateMoving();
	}
	//Pre:Need to update the sate of the car to stop moving right
	//Post: The state of the car has been updated, and is not moving right
	public void stopRight(){
		right = false;
		updateMoving();
	}
	//Pre:Need to know if the car is moving forward
	//Post:Returns true if the car is moving  forward false if it is not
	public boolean goingRight(){
		return right;
	}
	//Pre:Need to set the sate of the car to be moving forward
	//Post: The State of the car is now moving forward
	public void goFwd(){
		//set all other directions to false
		right = false;
		left = false;
		bckwd = false;
		fwd = true;
		updateMoving();
	}
	//Pre:Set the sate of the car to stop moving forward
	//Post:The state of the car has been set so it's not moving forward
	public void stopFwd(){
		fwd = false;
		updateMoving();
	}
	
	//Pre:Need to know if the car is moving forwards
	//Post:Returns true if moving forward false if not
	public boolean goingFwd(){
		return fwd;
	}
	//Pre:Need to tell the state that the car is now moving backwards
	//Post:The state of the car is now moving backwards
	public void goBckwd(){
		//set all other directions to false
		right = false;
		left = false;
		fwd = false;
		bckwd = true;
		updateMoving();
	}
	//Pre:Need to set the sate to not be moving backwards
	//Post:the state now sets moving backward to false
	public void stopBckwd(){
		bckwd = false;
		updateMoving();
	}
	//Pre:Need to know if the car is moving backward
	//Post:Retruns true if movng backward false if not
	public boolean goingBckwd(){
		return bckwd;
	}
	//Pre:Need to know if the car is in a moving state
	//Post:Return if the car is in a moving state or not
	public boolean getMoving(){
		return moving;
	}
	//Pre:Want to set the car to manual mode	
	//Post:The car's sate has been set to automatic mode
	public void setToManual(){
		manual = true;
	}
	//Pre:Want to set the car model to automatic mode
	//Post: The car's state has been set to automatic
	public void setToAtomatic(){
		manual = false;
	}
	//Pre:Need to know if manual is true or false
	//Post: Return the state of manual
	public boolean getManual(){
		return manual;
	}
	
	
	
}
