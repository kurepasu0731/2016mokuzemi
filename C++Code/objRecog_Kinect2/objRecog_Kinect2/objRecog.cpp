#include "KinectApp.h"

bool KinectApp::objRecogFunc(ObjectInfo& obj) {
	//return true; // debug�p

	if (obj.id == BANANA) { // �d�S�C��`���S����X������ӂɋ��߂�
		if (obj.size > objVal[BANANA].size_min && obj.size < objVal[BANANA].size_max) {
			// �~�����p�x�����̕K�v����A��̓I�ɂ� 17PAI/8 �̕���
			obj.dir = (int)(((atan2((double)(obj.g2Color.y - obj.sqo.y), (double)(obj.g2Color.x - obj.sqo.x)) + M_PI * 17. / 8.) / M_PI) * 4) % 8;
			return (obj.dir != -1);
		}
		else return false;
	} else {
		//return true; // debug�p
		// ����,�ʐς�F���畨�̂̎�ނ����
		if (!findObjectID(obj)) return false;

		// ��`�T�C�Y�Əی��ʐς����������
		switch (obj.id) {
		// TUNNEL��BRIDGE��0~3, -1(�O��l)�Ō����\��
		case TUNNEL:
		case BRIDGE:
			// obj.dir = 0or2�̂Ƃ�
			if (((double)obj.sq.y / (double)obj.sq.x) > objVal[obj.id].paramA_min) {
				obj.dir = 2;
			} else if(((double)obj.sq.x / (double)obj.sq.y) > objVal[obj.id].paramA_min) {
				obj.dir = 0;
			} else // obj.dir = 1or3�̂Ƃ�
				obj.dir = 1 + ((obj.ssize[0] + obj.ssize[2]) < (obj.ssize[1] + obj.ssize[3])) * 2;
			
			return true;
		case APPLE: 
		case PET: 
		case CAN:
			return true;
		default:
			return false;
		}
	}
	return false;
}

bool KinectApp::findObjectID(ObjectInfo& obj) {
	if (obj.h > objVal[APPLE].height_min && obj.h < objVal[APPLE].height_max &&
		obj.size > objVal[APPLE].size_min && obj.size < objVal[APPLE].size_max) {
		if ((obj.color[0] > objVal[APPLE].H_min || obj.color[0] < objVal[APPLE].H_max) &&
			(obj.color[1] > objVal[APPLE].S_min && obj.color[1] < objVal[APPLE].S_max) &&
			(obj.color[2] > objVal[APPLE].V_min && obj.color[2] < objVal[APPLE].V_max)) {

			obj.id = APPLE;
			return true;
		}
	} else if (obj.h > objVal[PET].height_min && obj.h < objVal[PET].height_max &&
		obj.size > objVal[PET].size_min && obj.size < objVal[PET].size_max) {
		if (obj.color[0] > objVal[PET].H_min && obj.color[0] < objVal[PET].H_max &&
			obj.color[1] > objVal[PET].S_min && obj.color[1] < objVal[PET].S_max &&
			obj.color[2] > objVal[PET].V_min && obj.color[2] < objVal[PET].V_max) {

			obj.id = PET;
			return true;
		}
	} else if (obj.h > objVal[CAN].height_min && obj.h < objVal[CAN].height_max &&
		obj.size > objVal[CAN].size_min && obj.size < objVal[CAN].size_max) {
		if (obj.color[0] > objVal[CAN].H_min && obj.color[0] < objVal[CAN].H_max &&
			obj.color[1] > objVal[CAN].S_min && obj.color[1] < objVal[CAN].S_max &&
			obj.color[2] > objVal[CAN].V_min && obj.color[2] < objVal[CAN].V_max) {

			obj.id = CAN;
			return true;
		}
	} else if (obj.h > objVal[TUNNEL].height_min && obj.h < objVal[TUNNEL].height_max &&
		obj.size > objVal[TUNNEL].size_min && obj.size < objVal[TUNNEL].size_max) {
		if (obj.color[0] > objVal[TUNNEL].H_min && obj.color[0] < objVal[TUNNEL].H_max &&
			obj.color[1] > objVal[TUNNEL].S_min && obj.color[1] < objVal[TUNNEL].S_max &&
			obj.color[2] > objVal[TUNNEL].V_min && obj.color[2] < objVal[TUNNEL].V_max) {

			obj.id = TUNNEL;
			return true;
		}
	} else if (obj.h > objVal[BRIDGE].height_min && obj.h < objVal[BRIDGE].height_max &&
		obj.size > objVal[BRIDGE].size_min && obj.size < objVal[BRIDGE].size_max) {
		if (obj.color[0] > objVal[BRIDGE].H_min && obj.color[0] < objVal[BRIDGE].H_max &&
			obj.color[1] > objVal[BRIDGE].S_min && obj.color[1] < objVal[BRIDGE].S_max &&
			obj.color[2] > objVal[BRIDGE].V_min && obj.color[2] < objVal[BRIDGE].V_max) {

			obj.id = BRIDGE;
			return true;
		}
	}
	return false;
}