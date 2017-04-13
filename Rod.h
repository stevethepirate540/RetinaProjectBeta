#pragma once
#include "stdafx.h"
#include "Photoreceptor.h"
#ifndef RODCELL
#define RODCELL

class Rod : public Photoreceptor {
public:
	Rod();
	virtual const char getType() override;
	virtual const char getRGB() override;
	virtual void update(float elapsedSeconds) override;
};

#endif