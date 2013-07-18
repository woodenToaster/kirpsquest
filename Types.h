/** @file Types.h */

#ifndef KQ_TYPES_H
#define KQ_TYPES_H

#include <stdint.h>

// main classes
class MainLoop;
class Screen;
class QuestProperties;
class Settings;
class KeysEffect;

// low level
class System;
class FileTools;
class VideoManager;
class Surface;
class TextSurface;
class Color;
class Sound;
class Music;
class SpcDecoder;
class ItDecoder;
class Random;
class Geometry;
class Rectangle;
class PixelBits;
class InputEvent;
class Debug;
class StringConcat;

// menus
class CustomScreen;
class CustomMenu;
class StringResource;

// game
class Game;
class GameCommands;
class Savegame;
class Equipment;
class EquipmentItem;
class EquipmentItemUsage;
class DebugKeys;
class Dungeon;
class Treasure;
class Map;
class MapLoader;
class Timer;
class GameoverSequence;
class Camera;
class Dialog;
class DialogResource;
class DialogBox;

// Lua
class ExportableToLua;
class LuaContext;

// drawable objects
class Sprite;
class SpriteAnimationSet;
class SpriteAnimation;
class SpriteAnimationDirection;
class Drawable;

// transition effects
class Transition;
class TransitionFade;
class TransitionImmediate;
class TransitionScrolling;

// map entities
class MapEntities;
class MapEntity;
class Hero;
class HeroSprites;
class Tile;
class DynamicTile;
class Detector;
class Teletransporter;
class Destination;
class Pickable;
class CarriedItem;
class Destructible;
class Chest;
class Jumper;
class Enemy;
class EnemyReaction;
class NPC;
class Block;
class Switch;
class Wall;
class Sensor;
class Crystal;
class CrystalBlock;
class ShopItem;
class Boomerang;
class Arrow;
class ConveyorBelt;
class Stairs;
class Door;
class Explosion;
class Bomb;
class Fire;
class Hookshot;

// tile patterns
class Tileset;
class TilePattern;
class SimpleTilePattern;
class AnimatedTilePattern;
class SelfScrollingTilePattern;
class TimeScrollingTilePattern;
class ParallaxScrollingTilePattern;

// movements
class Movement;
class StraightMovement;
class PlayerMovement;
class PixelMovement;
class FallingOnFloorMovement;
class JumpMovement;
class PathMovement;
class RandomPathMovement;
class PathFindingMovement;
class PathFinding;
class RandomMovement;
class FollowMovement;
class TargetMovement;
class CircleMovement;

#endif