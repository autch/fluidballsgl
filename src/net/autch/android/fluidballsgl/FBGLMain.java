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
import android.view.Window;

public class FBGLMain extends Activity implements SensorEventListener{
	private SensorManager	sensorManager;
	private Sensor			acceleroMeter;
	private GLSurfaceView	surfaceView;
	private FluidBallsRenderer renderer; 

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
}