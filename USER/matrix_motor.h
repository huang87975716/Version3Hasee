#ifndef __MATRIX_MOTOR_H__
	#define __MATRIX_MOTOR_H__
	unsigned char MotorDrive(int Dir, int Row,int Col, int TryTimes);
	void MotorRowDrive(int Dir, int Row);
	void MotorColDrive(int Dir, int Col);
	void MotorStopAll(void);
	void MotorDriveResverse(int Row);
	void MotorColStop(int Col);
	void MotorRowStop(int Row);
	unsigned char CheckMotorStatus(void);
#endif

