#include "sandbox.hpp"

#include "oasis.h"
#include "graphics/opengl/texture.hpp"

void SandboxLayer::Init() 
{
    Oasis::Reference<Oasis::AudioResource> audio = Oasis::ResourceManager::LoadResource<Oasis::AudioResource>("res/test.wav");
    Oasis::AudioSource * source = new Oasis::AudioSource();
    // source->Play(audio);

    Oasis::TextRenderer::LoadFont("res/Munro.ttf");
}

void SandboxLayer::Close() 
{

}

bool SandboxLayer::HandleEvent(const Oasis::Event& event)
{
    return false;
}

void SandboxLayer::Update()
{
    Oasis::Reference<Oasis::Texture> test = Oasis::ResourceManager::GetResource<Oasis::Texture>("res/animate.png");

    Oasis::Renderer::DrawLine(0.f, 0.f, 1280.f, 720.f, Oasis::Colour{0.f, 1.f, 1.f});
    Oasis::Renderer::DrawQuad(0.f, 0.f, 100.f, 100.f, Oasis::Colour{0.2f, 0.2f, 0.5f});
    Oasis::Renderer::DrawQuad(100.f, 100.f, 100.f, 100.f, test);

    Oasis::TextRenderer::DrawCharacter('c', 50.f, 50.f, Oasis::Colours::BLUE);
    Oasis::TextRenderer::DrawString("test string", 50.f, 100.f, Oasis::Colours::WHITE);

    Oasis::Sprite sprite("res/animate.png");
    sprite.SetPos(200.f, 200.f);
    sprite.SetDimensions(50.f, 50.f);
    sprite.SetSourcePos(30.f, 30.f);
    sprite.SetSourceDimensions(60.f, 60.f);
    Oasis::Renderer::DrawSprite(sprite);

    static bool initialized = false;
    static Oasis::AnimatedSprite sprite2("res/animate.png", 64.f, 80.f);
    if (!initialized)
    {
        initialized = true;
        sprite2.SetPos(300.f, 300.f);
        sprite2.SetDimensions(200.f, 200.f);
        sprite2.AddAnimation("default", 0, 1);
        sprite2.PlayAnimation("default");
    }
    Oasis::Renderer::DrawAnimatedSprite(sprite2);

}