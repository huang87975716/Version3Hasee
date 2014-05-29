#ifndef __MATRIX_MOTOR_H__
	#define __MATRIX_MOTOR_H__
	void MotorDrive(int Dir, int Row,int Col);
	void MotorRowDrive(int Dir, int Row);
	void MotorColDrive(int Dir, int Col);
	void MotorStopAll(void);
	void MotorDriveResverse(int Row);
	void MotorColStop(int Col);
	void MotorRowStop(int Row);
	extern unsigned char I2CValue2;
#endif

