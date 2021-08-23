#pragma once

#include <algorithm>
#include <cassert>
#include <map>
#include <type_traits>
#include <limits>

template<typename K, K DIFF = 1, std::enable_if_t<std::is_integral_v<K>, int> = 0>
class range_map_key
{
public:
	static constexpr K diff_v = DIFF;

public:
	range_map_key(K begin, K end) : _begin(begin), _end(end)
	{
		assert(_end >= begin);
	}

	range_map_key(K single)
		: _begin(single), _end(single)
	{
	}

	K begin() const
	{
		return _begin;
	}
	K end() const
	{
		return _end;
	}

	bool include(K value) const
	{
		return begin() <= value && value <= end();
	}

	bool include(const range_map_key<K>& rhs) const
	{
		return begin() <= rhs.begin() && rhs.end() <= end();
	}

	bool same(const range_map_key<K>& rhs) const
	{
		return begin() == rhs.begin() && end() == rhs.end();
	}

	bool lower(const range_map_key<K>& rhs) const
	{
		return begin() <= rhs.begin() && end() < rhs.end();
	}

	bool upper(const range_map_key<K>& rhs) const
	{
		return rhs.begin() < begin() && rhs.end() <= end();
	}

	range_map_key<K> merge(const range_map_key<K>& rhs) const
	{
		return range_map_key<K>(std::min(begin(), rhs.begin()), std::max(end(), rhs.end()));
	}

	bool operator>(const range_map_key<K>& rhs) const
	{
		return begin() > rhs.end();
	}
	bool operator==(const range_map_key<K>& rhs) const
	{
		return include(rhs.begin()) || include(rhs.end()) || same(rhs) || rhs.include(begin()) || rhs.include(end());
	}
	bool operator>=(const range_map_key<K>& rhs) const
	{
		return operator>(rhs) || operator==(rhs);
	}
	bool operator<(const range_map_key<K>& rhs) const
	{
		return !(operator>=(rhs));
	}
	bool operator<=(const range_map_key<K>& rhs) const
	{
		return !(operator>(rhs));
	}

private:
	K _begin;
	K _end;
};

template<typename K, typename V>
class range_map : public std::map<range_map_key<K>, V>
{
public:
	using base_map = std::map<range_map_key<K>, V>;
	using key_type = range_map_key<K>;
	using value_type = V;

public:
	bool merge_insert(key_type range, V value)
	{
		auto iter = base_map::find(range);
		if (iter != std::map<range_map_key<K>, V>::end())
		{
			if (iter->second == value)
			{
				range_map_key<K> newRange = range.merge(iter->first);
				base_map::erase(iter);
				return merge_insert(newRange, value);
			}
			else
			{
				split_delete(range);
				return merge_insert(range, value);
			}
		}

		return base_map::insert(std::pair<range_map_key<K>, V>(range, value)).second;
	}

	bool split_delete(key_type range)
	{
		auto iter = base_map::find(range);
		if (iter != std::map<key_type, value_type>::end())
		{
			key_type oldKey = iter->first;
			value_type oldValue = std::move(iter->second);

			base_map::erase(oldKey);

			if (range.include(oldKey))
			{
				return true;
			}
			else if (range.lower(oldKey))
			{
				key_type newKey(range.end() + key_type::diff_v, oldKey.end());
				return base_map::insert(std::make_pair(newKey, std::move(oldValue))).second;
			}
			else if (range.upper(oldKey))
			{
				key_type newKey(oldKey.begin(), range.begin() - key_type::diff_v);
				return base_map::insert(std::make_pair(newKey, std::move(oldValue))).second;
			}
			else if (oldKey.include(range))
			{
				key_type lowerKey(oldKey.begin(), range.begin() - key_type::diff_v);
				if (base_map::insert(std::make_pair(lowerKey, oldValue)).second)
				{
					key_type upperKey(range.end() + key_type::diff_v, oldKey.end());
					return base_map::insert(std::make_pair(upperKey, std::move(oldValue))).second;
				}
			}
			else
			{
				assert(false);
			}
		}

		return false;
	}
};
