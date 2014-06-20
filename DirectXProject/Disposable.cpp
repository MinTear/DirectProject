#include "stdafx.h"
#include "Disposable.h"


Disposable::Disposable()
{
	Disposables->insert(this);
}


Disposable::~Disposable()
{
}


void Disposable::Dispose()
{
	if (!IsDisposed())
	{
		OnDispose();
		Disposables->erase(this);
	}
}


bool Disposable::IsDisposed()
{
	return isDisposed;
}