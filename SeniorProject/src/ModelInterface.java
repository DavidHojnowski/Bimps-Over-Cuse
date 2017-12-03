
public interface ModelInterface {
	//This interface will be used in order to control what methods and data can be called on the model
	public void brake();
	public void setSpeed(int speed);
	public int getSpeed();
	public void speedUp();
	public void speedDown();
	public void goLeft();
	public void stopLeft();
	public boolean goingLeft();
	public void goRight();
	public void stopRight();
	public boolean goingRight();
	public void goFwd();
	public void stopFwd();
	public boolean goingFwd();
	public void goBckWd();
	public void stopBckWd();
	public boolean goingBckWd();
	public boolean isMoving();
	public boolean inManual();
	public void setManual();
	public void setAutomatic();

}
