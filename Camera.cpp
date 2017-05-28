#include "Camera.h"
#include <iostream>

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Orient(Point eye, Point focus, Vector up) {
	Vector look = focus - eye;
	Orient(eye, look, up);
}

void Camera::Orient(Point eye, Vector look, Vector up) {
	camEye = eye;
	camLook = normalize(look);
	camUp = normalize(up);

	w = -camLook;
	u = cross(camUp, w);
	v = cross(w, u);
}

void Camera::Reset() {
	Point eye = Point (0,0,0);
	Vector look = Vector (0, 0, 1);
	Vector up = Vector(0, 1, 0);
	Orient(eye, look, up);

	double defaultNear = 0.001;
	double defaultFar = 30.000;
	SetNearPlane(defaultNear);
	SetFarPlane(defaultFar);

	double defaultViewAngle = 45;
	SetViewAngle(defaultViewAngle);	

	int defaultScreenWidth = 480;
	int defaultScreenHeight = 480;
	SetScreenSize(defaultScreenWidth, defaultScreenHeight);
}

Matrix Camera::GetProjectionMatrix() {
	return GetUnhingeMatrix() * GetScaleMatrix();
}

/* viewAngle is mapped to height angle */
void Camera::SetViewAngle (double viewAngle) {
	camViewAngle = viewAngle;
}

void Camera::SetNearPlane (double nearPlane) {
	camNearPlane = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
	camFarPlane = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
	camScreenWidth = screenWidth;
	camScreenHeight = screenHeight;
}

Matrix Camera::GetUnhingeMatrix() {
	double c = -camNearPlane / camFarPlane;
	return Matrix(1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, -1 / (c + 1), c / (c + 1),
				  0, 0, -1, 0);
}

Matrix Camera::GetScaleMatrix() {
	double heightAngle = DEG_TO_RAD(camViewAngle);
	double a = 1 / (tan(heightAngle / 2) * camFarPlane * GetScreenWidthRatio());
	double b = 1 / (tan(heightAngle / 2) * camFarPlane);
	double c = 1 / camFarPlane;

	return Matrix(a, 0, 0, 0,
				  0, b, 0, 0,
				  0, 0, c, 0,
				  0, 0, 0, 1);
}

Matrix Camera::GetTranslationMatrix() {
	return Matrix(1, 0, 0, -camEye[0],
				  0, 1, 0, -camEye[1],
				  0, 0, 1, -camEye[2],
				  0, 0, 0, 1);
}

Matrix Camera::GetRotationMatrix() {
	return Matrix(u[0], u[1], u[2], 0,
				  v[0], v[1], v[2], 0,
				  w[0], w[1], w[2], 0,
				  0,    0,    0,    1);
}

Matrix Camera::GetModelViewMatrix() {
	return GetRotationMatrix() * GetTranslationMatrix();
}

void Camera::RotateU(double angle) {
	angle = DEG_TO_RAD(-angle);
	Matrix u_mat = rot_mat(u, angle);

	w = u_mat * w;
	v = u_mat * v;
}

void Camera::RotateV(double angle) {
	angle = DEG_TO_RAD(-angle);
	Matrix v_mat = rot_mat(v, angle);

	u = v_mat * u;
	w = v_mat * w;
}

void Camera::RotateW(double angle) {
	angle = DEG_TO_RAD(-angle);
	Matrix w_mat = rot_mat(w, angle);

	u = w_mat * u;
	v = w_mat * v;
}

void Camera::Translate(const Vector &v) {
	camEye = camEye + v;
}

void Camera::Rotate(Point p, Vector axis, double degrees) {
	double angle = DEG_TO_RAD(degrees);
	Matrix axis_rot = rot_mat(axis, angle);

	Vector pv = Vector(p[0], p[1], p[2]);
	Point translatedEye = camEye - pv;
	translatedEye = axis_rot * translatedEye;
	camEye = translatedEye + pv;
}


Point Camera::GetEyePoint() {
	return camEye;
}

Vector Camera::GetLookVector() {
	return camLook;
}

Vector Camera::GetUpVector() {
	return camUp;
}

double Camera::GetViewAngle() {
	return camViewAngle;
}

double Camera::GetNearPlane() {
	return camNearPlane;
}

double Camera::GetFarPlane() {
	return camFarPlane;
}

int Camera::GetScreenWidth() {
	return camScreenWidth;
}

int Camera::GetScreenHeight() {
	return camScreenHeight;
}

double Camera::GetFilmPlanDepth() {
	return camNearPlane;
}

double Camera::GetScreenWidthRatio() {
	return (double) camScreenWidth / (double) camScreenHeight;
}

Vector Camera::GetU() {
	return u;
}

Vector Camera::GetV() {
	return v;
}
Vector Camera::GetW() {
	return w;
}
