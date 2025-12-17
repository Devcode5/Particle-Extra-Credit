
// src/Engine.cpp
#include "Engine.h"

Engine::Engine()
{
    m_Window.create(VideoMode(1920, 1080), "Particles");
}

void Engine::run()
{
    Clock clock;
    cout << "Starting Particle Sim..." << endl;
    
    // Call Unit Tests once at start (Optional, based on your assignment)
    // Particle p(m_Window, 5, Vector2i(0,0)); 
    // p.unitTests();

    while (m_Window.isOpen())
    {
        Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();
        input();
        update(dtAsSeconds);
        draw();
    }
}

void Engine::input()
{
    Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            m_Window.close();
        }
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape) m_Window.close();
        }
        if (event.type == Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == Mouse::Left)
            {
                int numPoints = (rand() % 25) + 25;
                Vector2i mousePos = Mouse::getPosition(m_Window);
                // Spawn 5 particles per click for fun
                for(int i=0; i<5; i++) {
                    m_particles.push_back(Particle(m_Window, numPoints, mousePos));
                }
            }
        }
    }
}

void Engine::update(float dt)
{
    // Capture Mouse for Gravity Well
    Vector2i mousePixel = sf::Mouse::getPosition(m_Window);
    View view; 
    view.setCenter(0,0); 
    view.setSize(m_Window.getSize().x, -m_Window.getSize().y); // Invert Y for cartesian
    Vector2f mouseCoords = m_Window.mapPixelToCoords(mousePixel, view);

    bool attract = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    for (auto it = m_particles.begin(); it != m_particles.end(); )
    {
        if (it->getTTL() > 0.0)
        {
            // PASS NEW ARGUMENTS
            it->update(dt, mouseCoords, attract, (float)m_Window.getSize().y);
            it++;
        }
        else
        {
            it = m_particles.erase(it);
        }
    }
}

void Engine::draw()
{
    m_Window.clear();
    for (const auto& particle : m_particles)
    {
        m_Window.draw(particle);
    }
    m_Window.display();
}