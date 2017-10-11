
public class Model implements ModelInterface{

	Car ourCar;
	
	
	public Model(){
		ourCar = new Car();
	}


	@Override
	public void brake() {
		ourCar.brake();
		
	}


	@Override
	public void setSpeed(int speed) {
		ourCar.setSpeed(speed);
		
	}


	@Override
	public int getSpeed() {
		return ourCar.getSpeed();
	}


	@Override
	public void speedUp() {
		ourCar.speedUp();		
	}


	@Override
	public void speedDown() {
		ourCar.speedDown();
		
	}


	@Override
	public void goLeft() {
		ourCar.goLeft();
		
	}


	@Override
	public void stopLeft() {
		ourCar.stopLeft();
		
	}


	@Override
	public boolean goingLeft() {
		return ourCar.goingLeft();
	}


	@Override
	public void goRight() {
		ourCar.goRight();
		
	}


	@Override
	public void stopRight() {
		ourCar.stopRight();
		
	}


	@Override
	public boolean goingRight() {
		return ourCar.goingRight();
	}


	@Override
	public void goFwd() {
		ourCar.goFwd();
		
	}


	@Override
	public void stopFwd() {
		ourCar.stopFwd();
		
	}


	@Override
	public boolean goingFwd() {
		return ourCar.goingFwd();
	}


	@Override
	public void goBckWd() {
		ourCar.goBckwd();
		
	}


	@Override
	public void stopBckWd() {
		ourCar.stopBckwd();
		
	}


	@Override
	public boolean goingBckWd() {
		return ourCar.goingBckWd();
	}


	@Override
	public boolean isMoving() {
		return ourCar.getMoving();
	}
	
	
}
