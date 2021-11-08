#pragma once
#include<JuceHeader.h>
#include<complex>
typedef std::complex<float> complex;

template<int W>
class Spectrum : public juce::Component, juce::MouseListener {
private:
	// The result Fourier transform
	complex ft[W];
	// z
	complex z[W];
	// z^n
	complex zn[W];

	float freqs[W];

	float scale;
	juce::Image image;


	void CalculateTransform(float v) {
		for (int x = 0; x < W; x++) {
			ft[x] += v * zn[x];
			zn[x] *= z[x];
		}
	}

	float maxFreq = 20000;
	float minFreq = 20;
	float sr = 44100;
	void RecalculateZ() {
		for (int x = 0; x < W; x++) {
			//float wt = 3.14159 * x / (W - 1)
			float f = minFreq * pow(maxFreq / minFreq, (float)x / (W - 1));
			freqs[x] = f;
			float w = f*6.283/sr;
			z[x] = complex(cos(w) , sin(w));
		}
	}
	/*
	juce::Colour Complex2Color(complex c) {
		if (c != c)return juce::Colours::grey;
		float  t = pow(JUCE_LIVE_CONSTANT(0) + log(std::abs(c)), 3);
		t = t * 0.15 + 0.5;
		if (t > 1) { t = 1; }

		juce::Colour col = juce::Colour::fromHSL(std::arg(c) / 6.28318530718f, 0.3f, t, 1);
		return col;
	}*/
	float scaleY=10;
	void paint(juce::Graphics& g) override {

		g.setColour(juce::Colours::black);
		g.fillRect(getLocalBounds().toFloat());

		float width = getWidth();
		float height = getHeight();
		Component::paint(g);
		float lastY = log(abs(ft[0])) * -scaleY + height / 2;
		if ((lastY != lastY) || (lastY < 0))lastY = 0;
		if (lastY > height)lastY = height;
		float lastX = 0;
		
		g.setColour(juce::Colour(0,255,0));

		for (int x = 1; x < W; x++) {
			float X = width / (W - 1) * (x + 1);
			float Y =  log(abs(ft[x]))*-scaleY + height / 2;
			if ((Y!=Y)||(Y < 0))Y = 0;
			if (Y >height)Y = height;
			g.drawLine(lastX, lastY, X,Y,2);
			lastX = X;
			lastY = Y;
		}

		lastY = std::arg(ft[0]) / 6.28 * height + height / 2;
		lastX = 0;
		g.setColour(juce::Colours::orangered);
		for (int x = 1; x < W; x++) {
			float X = width / (W - 1) * (x + 1);
			float Y = std::arg(ft[x])/6.28*height + height / 2;
			if(abs(std::arg(ft[x]) -6.28- std::arg(ft[x-1]))> abs(std::arg(ft[x]) - std::arg(ft[x-1]))&& abs(6.28 + std::arg(ft[x]) - std::arg(ft[x-1])) > abs(std::arg(ft[x]) - std::arg(ft[x-1])))
				g.drawLine(lastX, lastY, X, Y, 2);
			lastX = X;
			lastY = Y;
		}

		
		g.setColour(juce::Colours::grey);
		float freqMark[]{20,50,100,200,500,1000,2000,5000,10000};
		int x = 0, flag = 0;
		for (int i = 0; i < sizeof(freqMark) / sizeof(freqMark[0]); i++) {
			while (freqs[x]<freqMark[i])
			{
				x++;
				if (!(x < W)) { flag = 1; break; }
			}
			if (!flag) {
				float X = width / (W - 1) * (x + 1);
				g.drawVerticalLine(X, 0, height);
				g.drawText(juce::String(freqMark[i]), X+2, height - 32, 90, 30, juce::Justification::bottomLeft);
			}
		}

		
		float ampMark[]{ 0.001,0.002,0.005,0.01,0.02,0.05,0.1,0.2,0.5,1,2,5,10,20,50,100,200,500 };
		int skip = std::max(1, (int)(6 - log(scaleY)));
		for (int i = 0; i < sizeof(ampMark) / sizeof(ampMark[0]); i+=skip) {
			float Y = log(ampMark[i]) * -scaleY + height / 2;
			if (Y<0 || Y>height)continue;
			g.drawHorizontalLine(Y, 0, width);
			g.drawText(juce::String(ampMark[i]), 5, Y-32, 90, 30, juce::Justification::bottomLeft);
		}

		

		g.setColour(juce::Colours::darkgrey);
		g.drawRect(getLocalBounds().toFloat(), 3);
	}

public:

	Spectrum() {
		RecalculateZ();
		Zero();
	}

	void AddSample(float sample) {
		CalculateTransform(sample);
	}

	void Zero() {
		for (int x = 0; x < W; x++) {
			ft[x]= 0;
			zn[x] = 1;
		}

	}
	void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override {
		scaleY *= pow(1.3, wheel.deltaY);
		repaint();
		//DBG(log(scaleY));
	}

};