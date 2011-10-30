package net.autch.android.fluidballsgl;

import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.widget.Toast;

public class FBGLMain extends Activity implements SensorEventListener{
	private SensorManager	sensorManager;
	private Sensor			acceleroMeter;
	private GLSurfaceView	surfaceView;
	private FluidBallsRenderer renderer;
	private final float accel[] = { 0.0f, 0.0f };
	private float dx = 0.05f, dy = 0.05f;
	private final float repeat_factor = 0.1f;
	private static final int MID_INVERT_X_POLAR = 0x1001;
	private static final int MID_INVERT_Y_POLAR = 0x1002;
	private static final int MID_SWAP_XY = 0x1003;
	private boolean swap_xy = false;
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		List<Sensor> list = sensorManager.getSensorList(Sensor.TYPE_ACCELEROMETER);
		if(list.size() > 0) acceleroMeter = list.get(0);

		getWindow().setFormat(PixelFormat.TRANSLUCENT);
		surfaceView = new GLSurfaceView(this);
		renderer = new FluidBallsRenderer();
		surfaceView.setRenderer(renderer);
		setContentView(surfaceView);
	}

	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// ignore
	}

	public void onSensorChanged(SensorEvent event) {
		if(event.sensor == acceleroMeter) {
			event.values[0] *= dx > 0 ? 1 : -1;
			event.values[1] *= dy > 0 ? 1 : -1;
			renderer.setAccel(event.values);
		}
	}

	@Override
	protected void onResume() {
		super.onResume();

		if(acceleroMeter != null)
			sensorManager.registerListener(this, acceleroMeter, SensorManager.SENSOR_DELAY_FASTEST);
		surfaceView.onResume();
	}

	@Override
	protected void onStop() {
		super.onStop();

		sensorManager.unregisterListener(this);

		surfaceView.onPause();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		float repeat;

		repeat = 1.0f; //repeat_factor * event.getRepeatCount();

		switch(keyCode) {
		case KeyEvent.KEYCODE_DPAD_LEFT:
			accel[0] -= dx * repeat;
			break;
		case KeyEvent.KEYCODE_DPAD_RIGHT:
			accel[0] += dx * repeat;
			break;
		case KeyEvent.KEYCODE_DPAD_UP:
			accel[1] -= dy * repeat;
			break;
		case KeyEvent.KEYCODE_DPAD_DOWN:
			accel[1] += dy * repeat;
			break;
		default:
			return super.onKeyDown(keyCode, event);
		}
		renderer.setAccel(accel);

		return true;
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		switch(keyCode) {
		case KeyEvent.KEYCODE_DPAD_LEFT:
		case KeyEvent.KEYCODE_DPAD_RIGHT:
			accel[0] = 0.0f;
			break;
		case KeyEvent.KEYCODE_DPAD_UP:
		case KeyEvent.KEYCODE_DPAD_DOWN:
			accel[1] = 0.0f;
			break;
		default:
			return super.onKeyUp(keyCode, event);
		}
		renderer.setAccel(accel);
		return true;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		menu.add(Menu.NONE, MID_SWAP_XY, Menu.NONE, "Swap X and Y");
		menu.add(Menu.NONE, MID_INVERT_X_POLAR, Menu.NONE, "Invert X polarity");
		menu.add(Menu.NONE, MID_INVERT_Y_POLAR, Menu.NONE, "Invert Y polarity");
		return true;
	}

	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		Toast t = null;

		switch(item.getItemId()) {
		case MID_SWAP_XY:
			swap_xy = !swap_xy;
			renderer.setSwap_xy(swap_xy);
			t = Toast.makeText(this, "Swapped X and Y", Toast.LENGTH_SHORT);
			t.show();
			break;
		case MID_INVERT_X_POLAR:
			dx = -dx;
			t = Toast.makeText(this, "Inverted X polarity", Toast.LENGTH_SHORT);
			t.show();
			break;
		case MID_INVERT_Y_POLAR:
			dy = -dy;
			t = Toast.makeText(this, "Inverted Y polarity", Toast.LENGTH_SHORT);
			t.show();
			break;
		}

		return super.onMenuItemSelected(featureId, item);
	}
}