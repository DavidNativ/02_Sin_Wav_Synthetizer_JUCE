#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
 
    frequencySlider.setRange(20, 10000, 10);
    frequencySlider.setTextValueSuffix("Hz");
    frequencySlider.onValueChange = [this]
    {
        if (currentSampleRate > 0.0)
            updateAngleDelta();
    };
    
    addAndMakeVisible(frequencySlider);

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
    auto level = 0.125f;
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
    frequencySlider.setBounds(labelSpace, 0, getWidth() - horizontalMargin, getHeight() - verticalMargin);

}


