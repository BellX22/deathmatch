#pragma once

enum class MessageType {
	None,
};

struct Message {
	MessageType type{MessageType::None};
	std::size_t sender{InvalidId};
	std::size_t receiver{InvalidId};
	sf::Time    delay;
};
