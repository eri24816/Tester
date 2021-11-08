#pragma once
#include<JuceHeader.h>
#include<complex>
typedef std::complex<float> complex;

template<int W>
class PoleZeroPlot : public juce::ImageComponent,juce::MouseListener {
private:
	//SafeQueue <float> buffer;

	// The result z transform
	complex zt[W][W];
	// z
	complex z[W][W];
	// z^n
	complex zn[W][W];

	float scale;
	juce::Image image;


	void CalculateZTransform(complex c) {
		for (int x = 0; x < W; x++) {
			for (int y = 0; y < W; y++) {
				zt[x][y] += c * zn[x][y];
				zn[x][y] *= z[x][y];
				
			}
		}

		if (std::abs(c) > 0) {
		}
	}

	void RecalculateZ() {
		for (int x = 0; x < W; x++) {
			for (int y = 0; y < W; y++) {
				z[x][y] = 1.f/(complex(((float)x+0.5) / W - 0.5f, -(((float)y+0.5) / W - 0.5f)) * scale);
			}
		}
	}

	juce::Colour Complex2Color(complex c) {
		if (c!=c)return juce::Colours::grey;
		float  t =  pow(JUCE_LIVE_CONSTANT(0)+log(std::abs(c)),3);
		t = t*0.15+0.5;
		if (t > 1) { t = 1; }
		
		juce::Colour col = juce::Colour::fromHSL(std::arg(c) / 6.28318530718f, 0.3f,t,1);
		return col;
	}

	void paint(juce::Graphics& g) override {
		ImageComponent::paint(g);
		setImage(image);
		for (int x = 0; x < W; x++) {
			for (int y = 0; y < W; y++) {
				image.setPixelAt(x, y, Complex2Color(zt[x][y]));
			}
		}
		setImage(image);

		g.saveState();

		float w = getLocalBounds().getWidth();
		float h = getLocalBounds().getHeight();
		float r = w > h ? h : w;

		// Transform component coords to complex plane coords
		g.addTransform(juce::AffineTransform::translation(w / 2, h / 2));
		g.addTransform(juce::AffineTransform::scale(r/scale));

		float lineWidth = scale / r;

		// The axis
		g.drawLine(-scale / 2, 0, scale / 2, 0, lineWidth);
		g.drawLine(0, -scale / 2, 0, scale / 2, lineWidth);

		// The unit circle
		g.setColour(juce::Colours::darkred);
		g.drawEllipse(juce::Rectangle<float>(-1,-1,2,2), lineWidth);

		g.restoreState();

		g.setColour(juce::Colours::darkgrey);
		g.drawRect(getLocalBounds().toFloat(), 3);
	}

public:

	PoleZeroPlot(float scale):scale(scale),image(juce::Image::PixelFormat::RGB,W,W,false) {
		RecalculateZ();
		Zero();
	}

	void AddSample(float sample) {
		CalculateZTransform(sample);
	}

	void Zero() {
		for (int x = 0; x < W; x++) {
			for (int y = 0; y < W; y++) {
				zt[x][y] = 0;
				zn[x][y] = 1;
			}
		}
		
	}
	void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override {
		scale *= pow(0.7,wheel.deltaY);
		RecalculateZ();
		repaint();
	}

	void resized() override {
		// Crop to square
		auto area = getBounds();
		float w = area.getWidth();
		float h = area.getHeight();
		float r = w > h ? h : w;
		setBounds(area.getProportion(juce::Rectangle<float>(
			(w-r)/(2*w), (h - r) / (2 * h),r/w,r/h
		)));
	}

};