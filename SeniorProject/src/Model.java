
public class Model implements ModelInterface{

	private Car ourCar;
	
	
	public Model(){
		ourCar = new Car();
	}


	@Override
	//Pre:Neeed to update the state of the car to refelect breaking and not moving
	//Post:The state of the car has been updated to be no longer moving 
	public void brake() {
		ourCar.brake();
		
	}


	@Override
	//Pre: Need to set the speed of the car to some known amount
	//Post: The speed has been set and uodated for the state of the car
	public void setSpeed(int speed) {
		ourCar.setSpeed(speed);
		
	}


	@Override
	//Pre:Need to get the curent speed of the sate of the car
	//Post: Returns the value of speed as an int
	public int getSpeed() {
		return ourCar.getSpeed();
	}


	@Override
	//Pre:Neeed to increase the speed of the car by some increment amount
	//Post: the speed has been increased by some set amount
	public void speedUp() {
		ourCar.speedUp();		
	}


	@Override
	//Pre: Need to decrease the speed of the sate of the car by an increment amount	
	//Post: The speed has been decreased by some set amount
	public void speedDown() {
		ourCar.speedDown();
		
	}


	@Override
	//Pre:Need to update the state of the car to move to the left
	//Post: The state of the car has been updated and is moving to the left
	public void goLeft() {
		ourCar.goLeft();
		
	}


	@Override
	//Pre:Need tot update the state to stop moving to the left
	//Post:The state of the car is no longer going to the left
	public void stopLeft() {
		ourCar.stopLeft();
		
	}


	@Override
	//Pre: Need to know if the car is moving to the left
	//Post: Retruns true if the car is moving to the left false if it is not 
	public boolean goingLeft() {
		return ourCar.goingLeft();
	}


	@Override
	//Pre:Need to set the sate of the car to be moving to the right
	//Post:The state of the car has been updated and is now moving to the right
	public void goRight() {
		ourCar.goRight();
		
	}


	@Override
	//Pre:Need to update the sate of the car to stop moving right
	//Post: The state of the car has been updated, and is not moving right
	public void stopRight() {
		ourCar.stopRight();
		
	}


	@Override
	//Pre:Need to know if the car is moving forward
	//Post:Returns true if the car is moving  forward false if it is not
	public boolean goingRight() {
		return ourCar.goingRight();
	}


	@Override
	//Pre:Need to set the sate of the car to be moving forward
	//Post: The State of the car is now moving forward
	public void goFwd() {
		ourCar.goFwd();
		
	}


	@Override
	//Pre:Set the sate of the car to stop moving forward
	//Post:The state of the car has been set so it's not moving forward
	public void stopFwd() {
		ourCar.stopFwd();
		
	}


	@Override
	//Pre:Need to know if the car is moving forwards
	//Post:Returns true if moving forward false if not
	public boolean goingFwd() {
		return ourCar.goingFwd();
	}


	@Override
	//Pre:Need to tell the state that the car is now moving backwards
	//Post:The state of the car is now moving backwards
	public void goBckWd() {
		ourCar.goBckwd();
		
	}


	@Override
	//Pre:Need to set the sate to not be moving backwards
	//Post:the state now sets moving backward to false
	public void stopBckWd() {
		ourCar.stopBckwd();
		
	}


	@Override
	//Pre:Need to know if the car is moving backward
	//Post:Retruns true if movng backward false if not
	public boolean goingBckWd() {
		return ourCar.goingBckwd();
	}


	@Override
	//Pre:Need to know if the car is in a moving state
	//Post:Return if the car is in a moving state or not
	public boolean isMoving() {
		return ourCar.getMoving();
	}


	@Override
	//Pre:Need to know if manual is true or false
	//Post: Return the state of manual
	public boolean inManual() {
		return ourCar.getManual();
	}


	@Override
	//Pre:Want to set the car to manual mode	
	//Post:The car's sate has been set to automatic mode
	public void setManual() {
		ourCar.setToManual();
		
	}


	@Override
	//Pre:Want to set the car model to automatic mode
	//Post: The car's state has been set to automatic
	public void setAutomatic() {
		ourCar.setToManual();
		
	}
	
	
}
