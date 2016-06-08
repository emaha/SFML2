#pragma once
class Shader
{
public:
	Shader();
	~Shader();

	virtual void apply(int output);
};

