////////////////////////////////////////////////////////////////////////
// Perso.cpp
// Super Mario Project
// Copyright (C) 2011  
// Lionel Joseph lionel.r.joseph@gmail.com
// Olivier Guittonneau openmengine@gmail.com
////////////////////////////////////////////////////////////////////////

#include "Perso.hpp"
#include <fstream>

namespace Collisions
{
	Perso::Perso(const string& textureName)  : EntityMovable(textureName), _environment(GROUND), _transformation(SMALL_MARIO), _state(STANDING),
			_hud(new HUD()), _canClimb(false), _acceleration(Vector2f()), _invincibleTime(0), _invincibleStarTime(0),
			_transformationTime(0), _attackTime(0), _specialAttackTime(0), _throwShellTime(0), _deathTime(0), _finishTime(0),
			_jumpTime(0) 
	{
		_keywords.push_back("marche");
		_keywords.push_back("course");
		_keywords.push_back("saut");
		_keywords.push_back("verticaux");
		_keywords.push_back("echelle");
		_keywords.push_back("marche_carapace");
		_keywords.push_back("carapace");
		_keywords.push_back("attaque");
		_keywords.push_back("attaque_speciale");

		loadPerso(textureName);
	}

	Perso::Perso(const string& textureName, Coord<float>& position) : EntityMovable(textureName, position), _environment(GROUND), _transformation(SMALL_MARIO), 
		_state(STANDING), _hud(new HUD()), _canClimb(false), _acceleration(Vector2f()), _invincibleTime(0), _invincibleStarTime(0),
			_transformationTime(0), _attackTime(0), _specialAttackTime(0), _throwShellTime(0), _deathTime(0), _finishTime(0),
			_jumpTime(0)
	{
		loadPerso(textureName);
	}

	HUD* Perso::getHUD()
	{
		return _hud;
	}

	Perso::State Perso::getState()
	{
		return _state;
	}

	Perso::Environment Perso::getEnvironment()
	{
		return _environment;
	}

	Perso::Transformations Perso::getTransformation()
	{
		return _transformation;
	}

	bool Perso::getCanClimb()
	{
		return _canClimb;
	}

	Vector2f Perso::getAcceleration()
	{
		return _acceleration;
	}

	MonsterOccurrence* Perso::getBroughtMonster()
	{
		return _broughtMonster;
	}

	Pipe* Perso::getInsidePipe()
	{
		return _insidePipe;
	}

	Checkpoint* Perso::getCheckPointPassed()
	{
		return _checkpointPassed;
	}

	int Perso::getInvincibleTime()
	{
		return _invincibleTime;
	}

	int Perso::getInvincibleStarTime()
	{
		return _invincibleStarTime;
	}

	int Perso::getTransformationTime()
	{
		return _transformationTime;
	}

	int Perso::getAttackTime()
	{
		return _attackTime;
	}

	int Perso::getSpecialAttackTime()
	{
		return _specialAttackTime;
	}

	int Perso::getThrowShellTime()
	{
		return _throwShellTime;
	}

	int Perso::getDeathTime()
	{
		return _deathTime;
	}

	int Perso::getFinishTime()
	{
		return _finishTime;
	}

	int Perso::getJumpTime()
	{
		return _jumpTime;
	}

	void Perso::setState(Perso::State state)
	{
		_state = state;
	}

	void Perso::setEnvironment(Environment environment)
	{
		_environment = environment;
	}

	void Perso::setTransformation(Transformations transformation)
	{
		_transformation = transformation;
	}

	void Perso::setCanClimb(bool canClimb)
	{
		_canClimb = canClimb;
	}

	void Perso::setBroughtMonster(MonsterOccurrence* monster)
	{
		_broughtMonster = monster;
	}

	void Perso::setInsidePipe(Pipe* pipe)
	{
		_insidePipe = pipe;
	}

	void Perso::setCheckPointPassed(Checkpoint* checkpoint)
	{
		_checkpointPassed = checkpoint;
	}

	void Perso::setInvincibleTime(int invincibleTime)
	{
		_invincibleTime = invincibleTime;
	}

	void Perso::setInvincibleStarTime(int invincibleStarTime)
	{
		_invincibleStarTime = invincibleStarTime;
	}

	void Perso::setTransformationTime(int transformationTime)
	{
		_transformationTime = transformationTime;
	}

	void Perso::setAttackTime(int attackTime)
	{
		_attackTime = attackTime;
	}

	void Perso::setSpecialAttackTime(int specialAttackTime)
	{
		_specialAttackTime = specialAttackTime;
	}

	void Perso::setThrowShellTime(int throwShellTime)
	{
		_throwShellTime = throwShellTime;
	}

	void Perso::setDeathTime(int deathTime)
	{
		_deathTime = deathTime;
	}

	void Perso::setFinishTime(int finishTime)
	{
		_finishTime = finishTime;
	}

	void Perso::setJumpTime(int jumpTime)
	{
		_jumpTime = jumpTime;
	}


	void Perso::update(RenderWindow& app)
	{
		gravity(&this->getSpeed(), app.GetFrameTime());
		lateral_move(app);

		/* Save actual position as previous position */
		_previousPosition = _position;

		/* Compute new position */
		this->setPosition(this->getPosition().getX() + app.GetFrameTime() * getSpeed().x, 
			this->getPosition().getY() + app.GetFrameTime() * getSpeed().y);

	}

	void Perso::lateral_move(RenderWindow& app)
	{
		int time = app.GetFrameTime();
		const Input& input = app.GetInput();

		if(_state != FINISH_CASTLE)
		{
			if(_state != FINISH)
			{
				if(input.IsKeyDown(Key::Right))
					_side = Side::RIGHT_SIDE;
				else
					_side = Side::LEFT_SIDE;

				if(input.IsKeyDown(Key::Right))
				{
					if(!input.IsKeyDown(Key::Down))
					{
						if(_speed.x < 0)
						{
							if(_environment != AIR && _broughtMonster == nullptr && _state != State::SKID)
							{
								_state = SKID;
								// play a sound here !
							}
							frictions(time);
						}
						else
						{
							if(_broughtMonster == nullptr)
							{
								if(_environment == GROUND)
								{
									if(_state != CLIMB_LADDER)
									{
										if(input.IsKeyDown(Key::B))
										{
											_state = RUN_1;
										}
										else
										{
											_state = WALK;
										}
										_hud->setNbMonstersKilled(0);
									}
								}
							}
							else
							{
								if(_environment == GROUND)
								{
									_state = WALK_SHELL;
								}
							}
						}
						_speed.x = _speed.x + _acceleration.x * time;
					}
				}
				else if(input.IsKeyDown(Key::Left))
				{
					if(!input.IsKeyDown(Key::Down))
					{
						if(_speed.x > 0)
						{
							if(_environment != AIR && _broughtMonster == nullptr && _state != State::SKID)
							{
								_state = SKID;
								// play a sound here !
							}
							frictions(time);
						}
						else
						{
							if(_broughtMonster == nullptr)
							{
								if(_environment == GROUND)
								{
									if(_state != CLIMB_LADDER)
									{
										if(input.IsKeyDown(Key::B))
										{
											_state = RUN_1;
										}
										else
										{
											_state = WALK;
										}
										_hud->setNbMonstersKilled(0);
									}
								}
							}
							else
							{
								if(_environment == GROUND)
								{
									_state = WALK_SHELL;
								}
							}
						}
						_speed.x = _speed.x - _acceleration.x * time;
					}
				}
				else if(!_specialAttackTime && !_attackTime){

					if(_broughtMonster == nullptr) 
					{
						if(_state == CLIMB_LADDER)
						{
							if(input.IsKeyDown(Key::Down))
								_speed.y = _speed.y - _acceleration.y * time;
							else if(input.IsKeyDown(Key::Up))
								_speed.y = _speed.y + _acceleration.y * time;
							else
								_speed.y = 0;

						}
						else
						{
							if(_environment == GROUND)
							{
								/*if(k->precedent[BAS] && k->actuel[BAS])*/
									_state = LOWERED_JUMP;
							}
							else
							{
								_state = STANDING;
								_hud->setNbMonstersKilled(0);
							}
						}
					}
					else
					{
						if(_environment == AIR)
						{
							//if(k->precedent[BAS] && k->actuel[BAS])
								_state = LOWERED_JUMP_SHELL;
						}
						else
						{
							_state = STANDING_SHELL;
							_hud->setNbMonstersKilled(0);
						}
					}
				}
			}

			if((!input.IsKeyDown(Key::Right) && !input.IsKeyDown(Key::Left))
				|| (input.IsKeyDown(Key::Down) && _environment == GROUND))
				frictions(time);
		}
		else
		{
			/* Character walks to castle without control on him */
			_speed.x = _acceleration.x * time;
		}
	}

	void Perso::frictions(float time)
	{
		float coeff;

		/* Frictions are different according to environment*/
		if(_environment == GROUND)
			coeff = CLASSIC_COEFF_FRICTION;
		else
			coeff = AIR_COEFF_FRICTION;

		if(time != 0)
		{
			_speed.x /= 1 + 5 * coeff * time * SLIDE_COEFF_FRICTION;
		}

		/* To avoid character moving suddenly of one pixel after a moment of immobility */
		if(_speed.x < SPEED_X_MIN && _speed.x > -SPEED_X_MIN)
			_speed.x = 0;
	}

	void Perso::updateInPipe()
	{
		switch(_insidePipe->getDirection())
		{
		case Pipe::Direction::TO_TOP:
			_state = State::GET_OUT_FROM_PIPE_VERTICAL;
			setPosition(_insidePipe->getPosition().getX() * BLOCK_WIDTH + BLOCK_WIDTH - getHitboxSize().x / 2, 
				_insidePipe->getPosition().getY() * BLOCK_WIDTH + (_insidePipe->getLenght() + 1) * BLOCK_WIDTH - getHitboxSize().y);
			break;

		case Pipe::Direction::TO_BOTTOM:
			_state = State::GET_OUT_FROM_PIPE_VERTICAL;
			setPosition(_insidePipe->getPosition().getX() * BLOCK_WIDTH + BLOCK_WIDTH - getHitboxSize().x / 2, 
				_insidePipe->getPosition().getY() * BLOCK_WIDTH);
			break;

		case Pipe::Direction::TO_LEFT:
			_side = Side::RIGHT_SIDE;
			_state = State::GET_OUT_FROM_PIPE_HORIZONTAL;
			setPosition((_insidePipe->getPosition().getX() + _insidePipe->getLenght()) * BLOCK_WIDTH, 
				_insidePipe->getPosition().getY() * BLOCK_WIDTH);
			break;

		case Pipe::Direction::TO_RIGHT:
			_side = Side::LEFT_SIDE;
			_state = State::GET_OUT_FROM_PIPE_HORIZONTAL;
			setPosition(_insidePipe->getPosition().getX() * BLOCK_WIDTH, _insidePipe->getPosition().getY() * BLOCK_WIDTH);
			break;

		default :
			break;
		}
		// Play sound pipe here !
	}

	void Perso::render(RenderWindow& app)
	{

	}

	void Perso::loadPerso(const string& textureName)
	{
		int abscisse_bas = 0, ordonnee_haut = 0, nb_sprites_max = 0;
		string fileName = textureName + ".perso";
		ifstream stream(fileName.c_str());
		vector<string>::iterator itKeywords;

		if(stream)
		{
			try
			{
				string word, wordToCompare;
				int value = INT_MAX;

				/* Important keywords */
				stream >> word;
				if(word == "abscisse_bas=")
				{
					stream >> abscisse_bas;
				}
				else
					throw exception(" \"abscisse_bas=\" keyword is missing");

				stream >> word;
				if(word == "ordonnee_haut=")
				{
					stream >> ordonnee_haut;
				}
				else
					throw exception(" \"ordonnee_haut=\" keyword is missing");

				// Optimization of reading file required each 'v_anim' follows 'nb_sprites'
				for(itKeywords = _keywords.begin(); itKeywords < _keywords.end(); itKeywords++)
				{
					stream >> word;
					wordToCompare = "nb_sprites_" + *itKeywords + "=";
					if(word == wordToCompare)
					{
						stream >> value;
						_spriteNumbersByState.push_back(value);
						
						// To compute Sprite size
						if(value > nb_sprites_max)
							nb_sprites_max = value;

						value = INT_MIN;
					}
					else
					{
						if((*itKeywords) != "attaque" || (*itKeywords) != "attaque_speciale")
						{
							wordToCompare = "\"" + wordToCompare;
							wordToCompare += "\" keyword is missing";
							throw exception(wordToCompare.c_str());
						}
					}

					stream >> word;
					wordToCompare = "v_anim" + *itKeywords + "=";
					if(word == wordToCompare)
					{
						stream >> value;
						_animationSpeeds.push_back(value);
						value = 0;
					}
					else
					{
						if((*itKeywords) != "attaque" || (*itKeywords) != "attaque_speciale")
						{
							wordToCompare = "\"" + wordToCompare;
							wordToCompare += "\" keyword is missing";
							throw exception(wordToCompare.c_str());
						}
					}
				}

				//stream >> word;
				//if(word == "nb_sprites_marche=")
				//{
				//	stream >> value;
				//	_spriteNumbersByState.push_back(value);
				//	value = INT_MAX;
				//}
				//else
				//	throw exception(" \"nb_sprites_marche=\" keyword is missing", 1);

				//stream >> word;
				//if(word == "nb_sprites_course=")
				//{
				//	stream >> value;
				//	_spriteNumbersByState.push_back(value);
				//	value = INT_MAX;
				//}
				//else
				//	throw exception(" \"nb_sprites_course=\" keyword is missing", 1);

				//stream >> word;
				//if(word == "nb_sprites_saut=")
				//{
				//	stream >> value;
				//	_spriteNumbersByState.push_back(value);
				//	value = INT_MAX;
				//}
				//else
				//	throw exception(" \"nb_sprites_saut=\" keyword is missing", 1);

				//stream >> word;
				//if(word == "nb_sprites_verticaux=")
				//{
				//	stream >> value;
				//	_spriteNumbersByState.push_back(value);
				//	value = INT_MAX;
				//}
				//else
				//	throw exception(" \"nb_sprites_verticaux=\" keyword is missing", 1);


				//stream >> word;
				//if(word == "nb_sprites_echelle=")
				//{
				//	stream >> value;
				//	_spriteNumbersByState.push_back(value);
				//	value = INT_MAX;
				//}
				//else
				//	throw exception(" \"nb_sprites_echelle=\" keyword is missing", 1);


				//stream >> word;
				//if(word == "nb_sprites_marche_carapace=")
				//{
				//	stream >> value;
				//	_spriteNumbersByState.push_back(value);
				//	value = INT_MAX;
				//}
				//else
				//	throw exception(" \"nb_sprites_marche_carapace=\" keyword is missing", 1);


				//stream >> word;
				//if(word == "nb_sprites_carapace=")
				//{
				//	stream >> value;
				//	_spriteNumbersByState.push_back(value);
				//	value = INT_MAX;
				//}
				//else
				//	throw exception(" \"nb_sprites_carapace=\" keyword is missing", 1);

				///* Optional keywords */
				//stream >> word;
				//if(word == "nb_sprites_attaque=")
				//{
				//	stream >> value;
				//	_spriteNumbersByState.push_back(value);
				//	value = INT_MAX;
				//}

				//stream >> word;
				//if(word == "nb_sprites_attaque_speciale=")
				//{
				//	stream >> value;
				//	_spriteNumbersByState.push_back(value);
				//	value = INT_MAX;
				//}
			}
			catch(exception& e)
			{
				cout << "Exception occured while reading " << fileName << " : " << e.what() << endl;
				getchar();
				exit(1);
			}

		}
		else
		{
			cout << "Exception occured while opening " << fileName << " : file doesn't exist." << endl;
			getchar();
			exit(1);
		}
	}


	Perso::~Perso()
	{
	}
} // namespace