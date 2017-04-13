#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

//int test = 0;
class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:

	ParticleSystem(unsigned int count,sf::Texture dat_texture,int p_size,int part_angle,float p_vel,float p_grav) :
		m_particles(count),
		m_vertices(sf::Points, count),
		m_lifetime(sf::seconds(2)),
		m_emitter(0, 0)
	{
		nicetexture = dat_texture;
		particle_size = p_size;
		p_angle = part_angle;
		particle_vel = p_vel;
		particle_grav = p_grav;
	}

	void particle_increase()
	{
		int num = m_particles.size();
		m_particles.resize(num + 100);
		m_vertices.resize(m_particles.size());
		printf(" "+ m_particles.size());
		
	}
	void particle_decrease()
	{
		if (m_particles.size() > 100)
		{
			int num = m_particles.size();
			m_particles.resize(num - 100);
			m_vertices.resize(m_particles.size());
			printf(" " + m_particles.size());
		}
	}

	void setEmitter(sf::Vector2f position)
	{
		m_emitter = position;
	}

	void update(sf::Time elapsed)
	{
		
		for (std::size_t i = 0; i < m_particles.size(); ++i)
		{
			// update the particle lifetime
			Particle& p = m_particles[i];
			p.lifetime -= elapsed;

			//gravity
			m_particles[i].velocity = m_particles[i].velocity + sf::Vector2f(0,particle_grav);

			


			// if the particle is dead, respawn it
			if (p.lifetime <= sf::Time::Zero)
				resetParticle(i);

			// update the position of the corresponding vertex
			//m_vertices[i].position += p.velocity * elapsed.asSeconds();
			p.corners[0].position += p.velocity * elapsed.asSeconds();
			p.corners[1].position += p.velocity * elapsed.asSeconds();
			p.corners[2].position += p.velocity * elapsed.asSeconds();

			// update the alpha (transparency) of the particle according to its lifetime
			float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
			p.corners[0].color.a = static_cast<sf::Uint8>(ratio * 255);
			p.corners[1].color.a = static_cast<sf::Uint8>(ratio * 255);
			p.corners[2].color.a = static_cast<sf::Uint8>(ratio * 255);
		}
	}

	~ParticleSystem()
	{

	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		
		

		states.texture = &nicetexture;
		for (int i = 0; i < m_particles.size(); i++)
		{
			
			target.draw(m_particles[i].corners, states);
		}
	}

private:

	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time lifetime;
		sf::VertexArray corners;
	};

	void resetParticle(std::size_t index)
	{
		// give a random velocity and lifetime to the particle
		float angle = 3.14f / 180.f*(std::rand() % (p_angle*2)+(90 - p_angle));
		//float angle = 3.14/180.f*270;
		float speed = (std::rand() % (int)(particle_vel*2)) + particle_vel;
		m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		//Particle Color
		//m_vertices[index].color = sf::Color((std::rand() % 255), (std::rand() % 255), (std::rand() % 255));
		m_particles[index].lifetime = sf::milliseconds((std::rand() % 1000) + 800);

		// reset the position of the corresponding vertex
		m_vertices[index].position = m_emitter;

		m_particles[index].corners = sf::VertexArray(sf::Triangles, 3);
		int p_size = (std::rand() % particle_size*2) + particle_size;
		m_particles[index].corners[0] = m_emitter + sf::Vector2f(0, -p_size);
		m_particles[index].corners[1] = m_emitter + sf::Vector2f(-p_size, p_size);
		m_particles[index].corners[2] = m_emitter + sf::Vector2f(p_size, p_size);

		m_particles[index].corners[0].texCoords = sf::Vector2f(5, 0);
		m_particles[index].corners[1].texCoords = sf::Vector2f(0, 10);
		m_particles[index].corners[2].texCoords = sf::Vector2f(10, 10);

		//m_particles[index].corners[0].color = sf::Color((std::rand() % 255), (std::rand() % 255), (std::rand() % 255));
		//m_particles[index].corners[1].color = m_particles[index].corners[0].color;
		//m_particles[index].corners[2].color = m_particles[index].corners[0].color;


		
	}
	int particle_size;
	float particle_vel;
	float particle_grav;
	int p_angle;
	std::vector<Particle> m_particles;
	sf::Texture nicetexture;
	sf::VertexArray m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
};


int main()
{
	//Emitter Positions
	sf::Vector2f emitter_position_one =  sf::Vector2f(500, 400);
	sf::Vector2f emitter_position_two = sf::Vector2f(500, 420);
	sf::Vector2f emitter_position_three = sf::Vector2f(500, 500);

	// create the window
	sf::RenderWindow window(sf::VideoMode(1000, 800), "Particles");

	//custom values for particle_system_one
	int particle_count=1000;
	int particle_size_top = 6;
	int particle_cone_angle = 60;
	float particle_vel = 50.f;
	float particle_grav = .6f;
	sf::Texture txture;
	if (!txture.loadFromFile("cooltexture.png"))
	{
		printf("1");
	}
	// create the particle system
	
	ParticleSystem p_system_one(particle_count,txture,particle_size_top,particle_cone_angle,particle_vel,particle_grav);
	particle_count = 1200;
	particle_size_top = 8;
	particle_cone_angle = 70;
	particle_vel = 60.f;
	particle_grav = .4f;
	if (!txture.loadFromFile("smoke.png"))
	{
		printf("1");
	}
	// create the particle system

	ParticleSystem p_system_two(particle_count, txture, particle_size_top, particle_cone_angle, particle_vel, particle_grav);

	particle_count = 800;
	particle_size_top = 10;
	particle_cone_angle = 60;
	particle_vel = 50.f;
	particle_grav = .3f;
	if (!txture.loadFromFile("smoke.png"))
	{
		printf("1");
	}
	// create the particle system

	ParticleSystem p_system_three(particle_count, txture, particle_size_top, particle_cone_angle, particle_vel, particle_grav);
	// create a clock to track the elapsed time
	sf::Clock clock;

	sf::Font font;
	sf::Text text;

	// select the font
	
	if (!font.loadFromFile("Pixeled.ttf"))
	{
		// error...
		printf("Error");
	}
	text.setFont(font);
	text.setString("UP - INCREASE P-COUNT\nDOWN - DECREASE P-COUNT");
	text.setPosition(20, 600);
	text.setCharacterSize(10);
	text.setFillColor(sf::Color::Red);

	if (!txture.loadFromFile("rocket.png"))
	{
		printf("1");
	}
	sf::Sprite sprite;
	sprite.setTexture(txture);
	sprite.setScale(.15,.15);
	sprite.setPosition(435,200);
	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) 
				window.close();
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up)
			{
				p_system_one.particle_increase();
				p_system_two.particle_increase();
				p_system_three.particle_increase();
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down)
			{
				p_system_one.particle_decrease();
				p_system_two.particle_decrease();
				p_system_three.particle_decrease();
				
			}
		}
		
		// make the particle system emitter follow the mouse
		//sf::Vector2i mouse = sf::Mouse::getPosition(window);
		p_system_one.setEmitter(emitter_position_one);
		p_system_two.setEmitter(emitter_position_two);
		p_system_three.setEmitter(emitter_position_three);
	

		// update it
		sf::Time elapsed = clock.restart();
		p_system_one.update(elapsed );
		p_system_two.update(elapsed);
		p_system_three.update(elapsed);

		// draw it
		window.clear();
		window.draw(p_system_three);
		window.draw(p_system_two);
		window.draw(p_system_one);
		window.draw(sprite);
		window.draw(text);
		window.display();
	}

	return 0;
}