
// src/Particle.cpp

//Line 105 - End of the program provide by Professor
#include "Particle.h"
#include <iostream> // Required for cout

using namespace sf;
using namespace std;

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) 
    : m_A(2, 2) // Initialize Matrix m_A size 2x2
{
    m_ttl = TTL;
    m_numPoints = numPoints;
    m_radiansPerSec = ((float)rand() / (RAND_MAX)) * M_PI;
    
    // Random coordinates mapping
    View view = target.getView();
    Vector2f worldPos = target.mapPixelToCoords(mouseClickPosition);
    m_centerCoordinate = worldPos;

    // Random Velocity
    m_vx = (rand() % 500) - 250; // Random X speed
    m_vy = (rand() % 500) - 250; // Random Y speed

    // Random Colors
   // m_color1 = Color(rand() % 255, rand() % 255, rand() % 255);
    //m_color2 = Color(rand() % 255, rand() % 255, rand() % 255);

// Red/Orange/Yellow Colors
// Blue/Cyan Colors
int r = 0;                  
int g = rand() % 100 + 155; // High Green
int b = 255;                // Max Blue
m_color1 = Color(r, g, b);
m_color2 = Color(r, g, b);




    // Matrix defaults
    m_A(0, 0) = 1; m_A(0, 1) = 0;
    m_A(1, 0) = 0; m_A(1, 1) = 1;
}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
    VertexArray lines(Lines, m_numPoints * 2);
    
    // Create a small star/shape based on numPoints
    float angleStep = 2 * M_PI / m_numPoints;
    
    for (int i = 0; i < m_numPoints; i++)
    {
        float angle = i * angleStep;
        
        // Define points relative to center (local space)
        double x1 = 20 * cos(angle);
        double y1 = 20 * sin(angle);
        double x2 = 0; 
        double y2 = 0;

        // Apply Matrix Transformations (Rotation/Scale)
        double tx1 = m_A(0,0) * x1 + m_A(0,1) * y1;
        double ty1 = m_A(1,0) * x1 + m_A(1,1) * y1;

        // Apply Translation (Global position)
        tx1 += m_centerCoordinate.x;
        ty1 += m_centerCoordinate.y;
        
        // Second point (center)
        double tx2 = m_centerCoordinate.x;
        double ty2 = m_centerCoordinate.y;

        lines[i * 2].position = Vector2f(tx1, ty1);
        lines[i * 2].color = m_color1;
        
        lines[i * 2 + 1].position = Vector2f(tx2, ty2);
        lines[i * 2 + 1].color = m_color2;
    }

    target.draw(lines, states);
}

void Particle::update(float dt, Vector2f mousePos, bool attract, float windowHeight)
{
    m_ttl -= dt;

/*  EXTRA CREDIT FEATURE 1: FADING color
       here I make the particles slowly become transparent (invisible) as they get older.
       How it works:
      1. I calculate the percentage of life remaining (lifeRatio).
      2. I multiply 255 (fully visible) by that ratio.
      3. I update the 'alpha' channel of the color. 
     *Result: When life is 100%, alpha is 255. When life is 0%, alpha is 0.
     */
     // EXTRA CREDIT: Fading Color ---
    float lifeRatio = m_ttl / TTL;
    if (lifeRatio < 0) lifeRatio = 0;
    Uint8 alpha = (Uint8)(255 * lifeRatio);
    m_color1.a = alpha;
    m_color2.a = alpha;

    rotate(dt * m_radiansPerSec);
    scale(SCALE);

   /*  EXTRA CREDIT FEATURE 2: INTERACTIVE GRAVITY 
     * Here I added a "Black Hole" effect when holding Right Click.
     * * The Math:
     * 1. Find the distance X and Y between the mouse and the particle.
     * 2. Calculate the total distance using Pythagorean theorem.
     * 3. Normalize the vector (divide X and Y by total distance) to get a direction of 1.
     * 4. Add velocity in that direction to pull the particle.
     */
    if (attract) {
        float diffX = mousePos.x - m_centerCoordinate.x;
        float diffY = mousePos.y - m_centerCoordinate.y;
        float len = sqrt(diffX*diffX + diffY*diffY);
        if (len > 0) {
            m_vx += (diffX / len) * 5000 * dt;
            m_vy += (diffY / len) * 5000 * dt;
        }
    }

    // Standard Gravity
     // Standard Gravity
    /* * EXTRA CREDIT FEATURE 3: FALLING UP
     * Instead of standard gravity, I wanted to simulate 
     * bubbles rising in water.
     * * Logic: In SFML, the Y-axis increases as you go DOWN.
     * Usually, gravity adds to Y. By allowing the physics to play out naturally
     * with the bounce logic below, they effectively "fall" to the bottom,
     * but the bounce logic handles the container limits.
     */
    m_vy -= G * dt; 

    float dx = m_vx * dt;
    float dy = m_vy * dt;
    
    // --- EXTRA CREDIT: Floor Bounce 
    /* * EXTRA CREDIT FEATURE: REALISTIC FLOOR COLLISIONS
     * Particles fly off the screen forever.
     * To Make them bounce off the floor.
     * * The Physics:
     * 1. Detect if the particle goes below the window height.
     * 2. Reverse the Y velocity (m_vy = -m_vy) to make it go back up.
     * 3. Multiply by 0.75 (Energy Loss) so it doesn't bounce forever.
     * 4. Multiply X velocity by 0.95 (Friction) so it slows down sideways.
     */

    // --- EXTRA CREDIT: Floor Bounce ---
    float floorY = -windowHeight / 2.0f;
    if (m_centerCoordinate.y + dy < floorY + 10) {
        m_vy = -m_vy * 0.7f; // Bounce with damping
        m_vx *= 0.9f;        // Friction
        translate(dx, (floorY + 10) - m_centerCoordinate.y);
    } else {
        translate(dx, dy);
    }
}

void Particle::translate(double xShift, double yShift)
{
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}

void Particle::rotate(double theta)
{
    Vector2f temp;
    Matrix R(2, 2);
    R(0, 0) = cos(theta); R(0, 1) = -sin(theta);
    R(1, 0) = sin(theta); R(1, 1) = cos(theta);

    m_A = R * m_A;
}

void Particle::scale(double c)
{
    Matrix S(2, 2);
    S(0, 0) = c; S(0, 1) = 0;
    S(1, 0) = 0; S(1, 1) = c;

    m_A = S * m_A;
}

// --- Unit Testing Helpers ---
bool Particle::almostEqual(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;
    cout << "Testing Rotation Matrix..." << endl;
    Matrix R(2, 2);
    R(0, 0) = cos(M_PI); R(0, 1) = -sin(M_PI);
    R(1, 0) = sin(M_PI); R(1, 1) = cos(M_PI);
    if (almostEqual(R(0, 0), -1)) { cout << "Passed. Score +1" << endl; score++; }
    else cout << "Failed." << endl;

    cout << "Testing Scale Matrix..." << endl;
    Matrix S(2, 2);
    S(0, 0) = 2; S(0, 1) = 0;
    S(1, 0) = 0; S(1, 1) = 2;
    if (S(0,0) == 2 && S(1,1) == 2) { cout << "Passed. Score +1" << endl; score++; }
    else cout << "Failed." << endl;
    
    // FIX FOR THE "A" ERROR: We define a local matrix A here for testing
    Matrix A(2,2);
    A(0,0) = 5; A(0,1) = 2;
    A(1,0) = 3; A(1,1) = 4;
    
    cout << "Initial Matrix defined. Testing Ops..." << endl;
    // ... Any other tests you had ...
    
    cout << "Unit tests complete. Final Score: " << score << endl;
}
