
#include <time.h>
#include "FluidBalls.h"
#include "net_autch_android_fluidballsgl_FluidBallsRenderer.h"

FluidBalls* pInstance = NULL;
jmethodID methodID = 0;
jclass klass = 0;

/*
 * Class:     net_autch_android_fluidballsgl_FluidBallsRenderer
 * Method:    fluidballs_init
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_autch_android_fluidballsgl_FluidBallsRenderer_fluidballs_1init
  (JNIEnv* env, jobject self, jint count, jint width, jint height)
{
	srand(time(NULL));
	pInstance = new FluidBalls(count, width, height);
	klass = (jclass)env->NewGlobalRef(env->GetObjectClass(self));
	methodID = env->GetMethodID(klass, "setBallCoord", "(FFF)V");
}

/*
 * Class:     net_autch_android_fluidballsgl_FluidBallsRenderer
 * Method:    fluidballs_exit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_autch_android_fluidballsgl_FluidBallsRenderer_fluidballs_1exit
  (JNIEnv* env, jobject self)
{
	delete pInstance;
	env->DeleteGlobalRef(klass);
	klass = 0;
	methodID = 0;
	pInstance = NULL;
}

/*
 * Class:     net_autch_android_fluidballsgl_FluidBallsRenderer
 * Method:    fluidballs_setaccel
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL Java_net_autch_android_fluidballsgl_FluidBallsRenderer_fluidballs_1setaccel
  (JNIEnv* env, jobject self, jfloat ax, jfloat ay)
{
	if(!pInstance) return;
	pInstance->setAccel(ax, ay);
}

/*
 * Class:     net_autch_android_fluidballsgl_FluidBallsRenderer
 * Method:    fluidballs_drawballs
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_autch_android_fluidballsgl_FluidBallsRenderer_fluidballs_1drawballs
  (JNIEnv* env, jobject self)
{
	if(!pInstance || !klass || !methodID) return;

	int count = pInstance->getCount();
	for(int i = 0; i < count; i++)
	{
		env->CallNonvirtualVoidMethod(self, klass, methodID,
				pInstance->getPX(i), pInstance->getPY(i), pInstance->getR(i));
	}
}

/*
 * Class:     net_autch_android_fluidballsgl_FluidBallsRenderer
 * Method:    fluidballs_getballpos
 * Signature: ([F)V
 */
JNIEXPORT void JNICALL Java_net_autch_android_fluidballsgl_FluidBallsRenderer_fluidballs_1getballpos
  (JNIEnv* env, jobject self, jfloatArray array)
{
	if(!pInstance) return;

	int count = pInstance->getCount();
	jint length = env->GetArrayLength(array);

	if(length < count * 2) return;

	jfloat* pArray = (jfloat*)env->GetPrimitiveArrayCritical(array, NULL);
	jfloat* p;
	if(pArray == NULL) return; // out of memory

	p = pArray;
	for(int i = 0; i < count; i++)
	{
		*p++ = pInstance->getPX(i);
		*p++ = pInstance->getPY(i);
	}

	env->ReleasePrimitiveArrayCritical(array, pArray, 0);
}

/*
 * Class:     net_autch_android_fluidballsgl_FluidBallsRenderer
 * Method:    fluidballs_update
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_autch_android_fluidballsgl_FluidBallsRenderer_fluidballs_1update
  (JNIEnv* env, jobject self)
{
	if(!pInstance) return;
	pInstance->update_balls();
}
