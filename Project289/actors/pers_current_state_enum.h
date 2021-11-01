#pragma once

enum class PersCurrentStateEnum {
	WalkLeft,
	WalkRight,
	WalkToward,
	WalkOutward,
	WalkLeftToward,
	WalkRightToward,
	WalkLeftOutward,
	WalkRightOutward,
	JumpLeft,
	JumpRight,
	JumpToward,
	JumpOutward,
	JumpLeftToward,
	JumpRightToward,
	JumpLeftOutward,
	JumpRightOutward,
	IdleLeft,
	IdleRight,
	IdleToward,
	IdleOutward,
	IdleLeftToward,
	IdleRightToward,
	IdleLeftOutward,
	IdleRightOutward,
	FireLeft,
	FireRight,
	FireToward,
	FireOutward,
	FireLeftToward,
	FireRightToward,
	FireLeftOutward,
	FireRightOutward,
	DieLeft,
	DieRight,
	DieToward,
	DieOutward,
	DieLeftToward,
	DieRightToward,
	DieLeftOutward,
	DieRightOutward
};

enum class PersCurrentStateClassEnum {
	Walk,
	Jump,
	Idle,
	Fire,
	Die
};

enum class PersCurrentOrientClassEnum {
	Left,
	Right,
	Toward,
	Outward,
	LeftToward,
	RightToward,
	LeftOutward,
	RightOutward
};