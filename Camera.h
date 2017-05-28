#ifndef CAMERA_H
#define CAMERA_H

#include "Algebra.h"

class Camera {
public:
	Camera();
	~Camera();
	void Orient(Point eye, Point focus, Vector up);
	void Orient(Point eye, Vector look, Vector up);
	void SetViewAngle (double viewAngle);
	void SetNearPlane (double nearPlane);
	void SetFarPlane (double farPlane);
	void SetScreenSize (int screenWidth, int screenHeight);
	void Reset();

	Matrix GetProjectionMatrix();
	Matrix GetModelViewMatrix();

	void RotateV(double angle);
	void RotateU(double angle);
	void RotateW(double angle);
	void Rotate(Point p, Vector axis, double degree);
	void Translate(const Vector &v);

	Vector GetU();
	Vector GetV();
	Vector GetW();

	Point GetEyePoint();
	Vector GetLookVector();
	Vector GetUpVector();
	double GetViewAngle();
	double GetNearPlane();
	double GetFarPlane();
	int GetScreenWidth();
	int GetScreenHeight();

	double GetFilmPlanDepth();
	double GetScreenWidthRatio();

private: 
	Matrix GetUnhingeMatrix();
	Matrix GetScaleMatrix();
	Matrix GetTranslationMatrix();
	Matrix GetRotationMatrix();

	double camViewAngle;
	double camNearPlane;
	double camFarPlane;
	int camScreenWidth, camScreenHeight;
	Point camEye;
	Vector camUp, camLook;
	Vector u, v, w;
};
#endif

