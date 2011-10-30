package net.autch.android.fluidballsgl;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLU;
import android.opengl.GLSurfaceView.Renderer;

public class FluidBallsRenderer implements Renderer {
	private static final int BALLS = 100;
	private final FloatBuffer circleBuffer;
	private final float[] buffer;
	private boolean swap_xy;

	public native void fluidballs_init(int count, int width, int height);
	public native void fluidballs_exit();
	public native void fluidballs_setaccel(float ax, float ay);
	public native void fluidballs_drawballs();
	public native void fluidballs_getballpos(float[] buffer);
	public native void fluidballs_update();

	static {
		System.loadLibrary("fluidballs");
	}

	public FluidBallsRenderer()
	{
		buffer = new float[BALLS * 2];

		ByteBuffer vbb = ByteBuffer.allocateDirect(BALLS * 2 * 4);
		vbb.order(ByteOrder.nativeOrder());
		circleBuffer = vbb.asFloatBuffer();
	}

	@Override
	public void onDrawFrame(GL10 gl)
	{
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);

		fluidballs_update();
		// fluidballs_drawballs();
		fluidballs_getballpos(buffer);
		circleBuffer.rewind();
		circleBuffer.put(buffer);
		circleBuffer.rewind();
		gl.glVertexPointer(2, GL10.GL_FLOAT, 0, circleBuffer);
		gl.glDrawArrays(GL10.GL_POINTS, 0, BALLS);
		gl.glFlush();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height)
	{
		gl.glViewport(0, 0, width, height);

		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();
		GLU.gluOrtho2D(gl, 0, width, height, 0);
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();

		fluidballs_exit();
		fluidballs_init(BALLS, width, height);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		gl.glClearColor(0, 0, 0, 1);
		gl.glColor4f(1, 1, 0, 1);
		gl.glHint(GL10.GL_POINT_SMOOTH_HINT, GL10.GL_NICEST);
		gl.glEnable(GL10.GL_POINT_SMOOTH);
		gl.glDisable(GL10.GL_LIGHTING);
		gl.glDisable(GL10.GL_DEPTH_TEST);
		gl.glPointSize(10);
	}

	// called from fluidballs_drawballs()
	private void setBallCoord(float xc, float yc, float r)
	{
		if(!swap_xy) {
			circleBuffer.put(xc); circleBuffer.put(yc);
		} else {
			circleBuffer.put(yc); circleBuffer.put(xc);
		}
	}

	public void setAccel(float[] values)
	{
		if(!swap_xy) {
			fluidballs_setaccel(-values[0] / 8.0f, values[1] / 8.0f);
		} else {
			fluidballs_setaccel(-values[1] / 8.0f, values[0] / 8.0f);
		}
	}
	
	public void setSwap_xy(boolean swap_xy)
	{
		this.swap_xy = swap_xy;
	}
}
