#pragma once

#ifdef BATTLEENG_EXPORTS
#define PLATFORM_API __declspec(dllexport)
#else 
#define PLATFORM_API __declspec(dllimport)
#endif

#include <random>
#include <type_traits>

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

enum class dirs { left = 0, right = 1, up = 2, down = 3, stop = 4 };
enum class actions {
	stop = 0, move = 1, flee = 2, harvest = 3, shoot = 4, melee = 5,
	mining = 6, return_gold = 7, return_wood = 8, spotted = 9
};
enum class unit_type {
	peon = 0, orc_warrior = 1, orc_archer = 2, orc_knight = 3, artillery = 4,
	peasant = 5, warrior = 6, archer = 7, knight = 8
};
enum class obstacle { rock = 0, small_tree = 1, mid_tree = 2, big_tree = 3, orc_castle = 4, human_castle = 5, mine = 6 };

struct FPOINT
{
	float x{ 0 };
	float y{ 0 };
};
struct FRECT
{
	FPOINT up_left{ 0 };
	FPOINT up_right{ 0 };
	FPOINT down_left{ 0 };
	FPOINT down_right{ 0 };
};
struct ACTPARAMS
{
	actions next_action{ actions::stop };
	int asset_involved{ -1 };
	int enemy_involved{ -1 };
};

namespace dll
{
	template<typename T> class BAG
	{
	private:
		T* m_ptr{ nullptr };
		size_t max_size{ 0 };
		size_t next_pos{ 0 };

	public:
		BAG() :max_size{ 1 }, m_ptr{ reinterpret_cast<T*>(calloc(max_size,sizeof(T))) } {};
		BAG(size_t lenght) :max_size{ lenght }, m_ptr{ reinterpret_cast<T*>(calloc(lenght,sizeof(T))) } {};
		BAG(const BAG<T>& other)
		{
			if (other.m_ptr == this->m_ptr || other.next_pos < 1)return;

			m_ptr = reinterpret_cast<T*>(calloc(other.max_size, sizeof(T)));
			max_size = other.max_size;
			next_pos = other.next_pos;

			for (size_t count = 0; count < next_pos; count++)m_ptr[count] = other.m_ptr[count];
		}

		~BAG()
		{
			if (m_ptr)free(m_ptr);
		}

		bool empty() const
		{
			if (next_pos == 0)return true;
			return false;
		}
		size_t size() const
		{
			return next_pos;
		}
		size_t capacity() const
		{
			return max_size;
		}

		bool push_back(T element)
		{
			if (m_ptr)
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = element;
					++next_pos;
					return true;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					if (m_ptr)
					{
						m_ptr[next_pos] = element;
						++next_pos;
						return true;
					}
				}
			}
			return false;
		}
		bool push_back(T* element)
		{
			if (m_ptr)
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = (*element);
					++next_pos;
					return true;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					if (m_ptr)
					{
						m_ptr[next_pos] = (*element);
						++next_pos;
						return true;
					}
				}
			}
			return false;
		}

		void push_front(T element)
		{
			(*m_ptr) = element;
		}
		void push_front(T* element)
		{
			(*m_ptr) = (*element);
		}

		void erase(size_t index)
		{
			if (index >= next_pos)return;

			if (m_ptr)
			{
				for (size_t count = index; count < next_pos - 1; ++count)m_ptr[count] = m_ptr[count + 1];

			}
		}

		T& operator [] (size_t index)
		{
			if (index >= next_pos) throw std::runtime_error("Wrong index !");
			if (!m_ptr) throw std::runtime_error("Invalid data pointer !");

			return m_ptr[index];
		}

		BAG& operator =(BAG& other)
		{
			if (m_ptr)
			{
				if (this->m_ptr == other.m_ptr)return other;

				if (this->max_size >= other.max_size)
				{
					for (size_t count = 0; count < other.next_pos; ++count)m_ptr[count] = other[count];
				}
				else
				{
					max_size = other.max_size;

					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					if (m_ptr)
					{
						for (size_t count = 0; count < other.next_pos; ++count)m_ptr[count] = other[count];
					}
				}
			}
		}
	};

	/// ////////////////////////////////////////////

	class PLATFORM_API RandIt
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RandIt();
		RandIt(RandIt& other);
		~RandIt();

		int operator()(int min, int max);
	};

	class PLATFORM_API PROTON
	{
	protected:
		float _width{ 0 };
		float _height{ 0 };

	public:
		FPOINT start{};
		FPOINT end{};
		FPOINT center{};

		float radiusX{ 0 };
		float radiusY{ 0 };

		PROTON();
		PROTON(float _sx, float _sy);
		PROTON(float _sx, float _sy, float _swidth, float _sheight);

		virtual ~PROTON() {};

		void SetEdges();
		void NewWidth(float _new_width);
		void NewHeight(float _new_height);
		void NewDims(float _new_width, float _new_height);

		float GetWidth() const;
		float GetHeight() const;
	};

	class PLATFORM_API ASSETS :public PROTON
	{
	private:

		static int global_counter;

		int number{ 0 };

		int goods_in_asset{ 0 };
		int goods_delay = 0;
		int max_goods_delay;

		int max_frames{ 0 };
		int frame{ 0 };
		int frame_delay{ 0 };
		int max_frame_delay{ 0 };

		ASSETS(obstacle what_type, float start_x, float start_y);

	public:
		obstacle type;
		int lifes = 0;

		int GetFrame();
		int GetMyNumber() const;
		int GetGoods();
		void Release();

		friend PLATFORM_API ASSETS* AssetFactory(obstacle what, float startx, float starty);
	};

	class PLATFORM_API UNITS :public PROTON
	{
	private:
		static int global_counter;

		int number{ 0 };

		float speed{ 0 };
		int strenght{ 0 };
		float range{ 0 };

		int max_frames{ 0 };
		int frame{ 0 };
		int frame_delay{ 0 };
		int max_frame_delay{ 0 };

		int attack_delay{ 0 };
		int max_attack_delay{ 0 };

		bool hor_path{ false };
		bool vert_path{ false };
		float slope{ 0 };
		float intercept{ 0 };

		float start_move_x{ 0 };
		float start_move_y{ 0 };
		float final_move_x{ 0 };
		float final_move_y{ 0 };

		UNITS(unit_type what, float start_x, float start_y);

	public:
		unit_type type{ unit_type::peon };
		dirs dir{ dirs::stop };
		actions current_action{ actions::stop };

		int lifes{ 0 };

		virtual ~UNITS() {};
		void Release();

		void SetPath(float _where_x, float _where_y);

		int GetFrame();
		int GetMyNumber() const;

		void Move(float gear, BAG<ASSETS>obstacles);
		int Attack();

		friend PLATFORM_API ACTPARAMS AINextMove(UNITS my_unit, BAG<UNITS>& BadArmy, BAG<ASSETS>& Obstacles, ACTPARAMS params);
		friend PLATFORM_API UNITS* UnitFactory(unit_type what_unit, float startx, float starty);
	};

	// FUNCTIONS *******************************************

	template<typename T>concept primes = std::is_same<T, int>::value || std::is_same<T, float>::value
		|| std::is_same<T, double>::value || std::is_same<T, long>::value || std::is_same<T, char>::value;

	template<primes T> void SortPrimes(BAG<T>& bag, bool ascending = true)
	{
		if (bag.size() > 1)
		{
			if (ascending)
			{
				bool is_ok = false;

				while (!is_ok)
				{
					is_ok = true;

					for (size_t count = 0; count < bag.size() - 1; ++count)
					{
						if (bag[count] > bag[count + 1])
						{
							T temp = bag[count];
							bag[count] = bag[count + 1];
							bag[count + 1] = temp;
							is_ok = false;
						}
					}
				}
			}
			else
			{
				bool is_ok = false;

				while (!is_ok)
				{
					is_ok = true;

					for (size_t count = 0; count < bag.size() - 1; ++count)
					{
						if (bag[count] < bag[count + 1])
						{
							T temp = bag[count];
							bag[count] = bag[count + 1];
							bag[count + 1] = temp;
							is_ok = false;
						}
					}
				}
			}
		}
	}

	bool PLATFORM_API Intersect(FRECT first, FRECT second);
	float PLATFORM_API Distance(FPOINT first, FPOINT second);
	void PLATFORM_API Sort(BAG<UNITS>& army, FPOINT target);

	PLATFORM_API ACTPARAMS AINextMove(UNITS my_unit, BAG<UNITS>& BadArmy, BAG<ASSETS>& Obstacles, ACTPARAMS params);

	PLATFORM_API ASSETS* AssetFactory(obstacle what, float startx, float starty);

	PLATFORM_API UNITS* UnitFactory(unit_type what_unit, float startx, float starty);
}