#pragma once

const short int zero[15] = {
	1,1,1,
	1,0,1,
	1,0,1,
	1,0,1,
	1,1,1};

const short int one[15] = {
	1,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	1,1,1};

const short int two[15] = {
	1,1,1,
	0,0,1,
	1,1,1,
	1,0,0,
	1,1,1};

const short int three[15] = {
	1,1,1,
	0,0,1,
	1,1,1,
	0,0,1,
	1,1,1};

const short int four[15] = {
	1,0,0,
	1,0,1,
	1,1,1,
	0,0,1,
	0,0,1};

const short int five[15] = {
	1,1,1,
	1,0,0,
	1,1,1,
	0,0,1,
	1,1,1};


const short int six[15] = {
	1,0,0,
	1,0,0,
	1,1,1,
	1,0,1,
	1,1,1};

const short int seven[15] = {
	1,1,1,
	0,0,1,
	0,0,1,
	0,0,1,
	0,0,1};

const short int eight[15] = {
	1,1,1,
	1,0,1,
	1,1,1,
	1,0,1,
	1,1,1};

const short int nine[15] = {
	1,1,1,
	1,0,1,
	1,1,1,
	0,0,1,
	0,0,1};

const short int *digits[10] = {
	zero,
	one,
	two,
	three,
	four,
	five,
	six,
	seven,
	eight,
	nine
};

