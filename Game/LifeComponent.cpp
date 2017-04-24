#include "LifeComponent.hpp"

#include "GameSingleton.hpp"
#include "GameConfig.hpp"

LifeComponent::LifeComponent(oe::Entity& entity)
	: oe::RenderableComponent(entity)
	, mLifeMax(0)
	, mLife(0)
{
	mBack.setOutlineColor(sf::Color::Black);
	mBack.setOutlineThickness(1.f);
	mBack.setFillColor(sf::Color::Red);
	mBack.setSize(sf::Vector2f(50.f, 5.f));
	mBar.setOutlineColor(sf::Color::Black);
	mBar.setOutlineThickness(1.f);
	mBar.setSize(sf::Vector2f(0.f, 5.f));
	mBar.setFillColor(sf::Color::Green);
}

void LifeComponent::setLifeMax(U32 lifeMax)
{
	mLifeMax = (F32)lifeMax;
	updateLife();
}

void LifeComponent::setLife(U32 life)
{
	mLife = (F32)life;
	updateLife();
}

void LifeComponent::render(sf::RenderTarget& target)
{
	if (!oe::Math::equals(mLife, mLifeMax))
	{
		sf::Transform t(getGlobalTransform());
		target.draw(mBack, t);
		target.draw(mBar, t);
	}
}

void LifeComponent::updateLife()
{
	if (mLifeMax > 0.0f)
	{
		F32 percent = mLife / mLifeMax;
		mBar.setSize(sf::Vector2f(percent * 50.f, 5.f));
	}
}
