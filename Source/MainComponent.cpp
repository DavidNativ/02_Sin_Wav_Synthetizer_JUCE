#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
 
    frequencySlider.setRange(20, 10000, 10);
    frequencySlider.setValue(440);
    frequencySlider.setTextValueSuffix("Hz");
    frequencySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    frequencySlider.setSkewFactor(0.5);
    frequencySlider.onValueChange = [this]
    {
        if (currentSampleRate > 0.0)
            updateAngleDelta();
    };
    

    volumeSlider.setRange(0, 1, 0.01);    
    volumeSlider.setTextValueSuffix("%");
    volumeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    volumeSlider.setSkewFactor(0.5);

    frequencyLabel.setText("Frequency",juce::NotificationType::dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, false);
    volumeLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    volumeLabel.attachToComponent(&volumeSlider, false);


    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(frequencyLabel);
    addAndMakeVisible(volumeLabel);

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 400);
    setAudioChannels(0, 2);

 
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}


//==============================================================================



void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    currentSampleRate = sampleRate;
    updateAngleDelta();
    

    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto level = (float) volumeSlider.getValue();
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        auto currentSample = (float)std::sin(currentAngle);
        currentAngle += angleDelta;
        leftBuffer[sample] = currentSample * level;
        rightBuffer[sample] = currentSample * level;

    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    const int labelSpace = 50;
    const int horizontalMargin = 100;
    const int verticalMargin = 50;
    frequencySlider.setBounds(labelSpace, 0, getWidth()/2 - horizontalMargin, getHeight() - verticalMargin);
    volumeSlider.setBounds(labelSpace + getWidth()/2, 0, getWidth()/2 - horizontalMargin, getHeight() - verticalMargin);

}


