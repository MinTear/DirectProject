#pragma once

#include "stdafx.h"
#include "resource.h"
#include "DeviceManager.h"

#define MAX_LOADSTRING 100
// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
DeviceManager dManager;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

/*class DirectXProject
{

public:
	// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
	ATOM				MyRegisterClass(HINSTANCE hInstance);
	BOOL				InitInstance(HINSTANCE hInstance, int nCmdShow);
	LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK	About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


};*/