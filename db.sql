-- MySQL dump 10.13  Distrib 9.3.0, for macos15.2 (arm64)
--
-- Host: localhost    Database: digitalshadows
-- ------------------------------------------------------
-- Server version	9.3.0

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `character_equipment`
--

DROP TABLE IF EXISTS `character_equipment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `character_equipment` (
  `id` int NOT NULL AUTO_INCREMENT,
  `char_id` int DEFAULT NULL,
  `armor` int DEFAULT '0',
  `helmet` int DEFAULT '0',
  `amulet` int DEFAULT '0',
  `ring` int DEFAULT '0',
  `gloves` int DEFAULT '0',
  `onhand` int DEFAULT '0',
  `offhand` int DEFAULT '0',
  `legs` int DEFAULT '0',
  `boots` int DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `character_inventory`
--

DROP TABLE IF EXISTS `character_inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `character_inventory` (
  `id` int NOT NULL AUTO_INCREMENT,
  `char_id` int DEFAULT NULL,
  `bag0` int DEFAULT '0',
  `bag1` int DEFAULT '0',
  `bag2` int DEFAULT '0',
  `bag3` int DEFAULT '0',
  `bag4` int DEFAULT '0',
  `bag5` int DEFAULT '0',
  `bag6` int DEFAULT '0',
  `bag7` int DEFAULT '0',
  `bag8` int DEFAULT '0',
  `bag9` int DEFAULT '0',
  `bag10` int DEFAULT '0',
  `bag11` int DEFAULT '0',
  `bag12` int DEFAULT '0',
  `bag13` int DEFAULT '0',
  `bag14` int DEFAULT '0',
  `bag15` int DEFAULT '0',
  `bag16` int DEFAULT '0',
  `bag17` int DEFAULT '0',
  `bag18` int DEFAULT '0',
  `bag19` int DEFAULT '0',
  `bag20` int DEFAULT '0',
  `bag21` int DEFAULT '0',
  `bag22` int DEFAULT '0',
  `bag23` int DEFAULT '0',
  `bag24` int DEFAULT '0',
  `bag25` int DEFAULT '0',
  `bag26` int DEFAULT '0',
  `bag27` int DEFAULT '0',
  `bag28` int DEFAULT '0',
  `bag29` int DEFAULT '0',
  `bag30` int DEFAULT '0',
  `bag31` int DEFAULT '0',
  `bag32` int DEFAULT '0',
  `bag33` int DEFAULT '0',
  `bag34` int DEFAULT '0',
  `bag35` int DEFAULT '0',
  `bag36` int DEFAULT '0',
  `bag37` int DEFAULT '0',
  `bag38` int DEFAULT '0',
  `bag39` int DEFAULT '0',
  `bag40` int DEFAULT '0',
  `bag41` int DEFAULT '0',
  `bag42` int DEFAULT '0',
  `bag43` int DEFAULT '0',
  `bag44` int DEFAULT '0',
  `bag45` int DEFAULT '0',
  `bag46` int DEFAULT '0',
  `bag47` int DEFAULT '0',
  `bag48` int DEFAULT '0',
  `bag49` int DEFAULT '0',
  `bag50` int DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `characters` (
  `id` int NOT NULL AUTO_INCREMENT,
  `user_id` int DEFAULT NULL,
  `name` varchar(15) DEFAULT NULL,
  `race` enum('Elf','Dwarf','Human','Orc') DEFAULT 'Human',
  `sex` enum('Male','Female') DEFAULT 'Male',
  `level` int DEFAULT '0',
  `experience` int DEFAULT '0',
  `max_health` int DEFAULT '50',
  `current_health` int DEFAULT '50',
  `max_mana` int DEFAULT '20',
  `current_mana` int DEFAULT '20',
  `base_strength` int DEFAULT '5',
  `base_endurance` int DEFAULT '5',
  `base_wisdom` int DEFAULT '5',
  `base_mystic` int DEFAULT '5',
  `base_agility` int DEFAULT '5',
  `base_accuracy` int DEFAULT '5',
  `base_intellect` int DEFAULT '5',
  `base_luck` int DEFAULT '5',
  `skill_sword` int DEFAULT '10',
  `skill_sword_exp` int DEFAULT '0',
  `skill_spear` int DEFAULT '10',
  `skill_spear_exp` int DEFAULT '0',
  `skill_axe` int DEFAULT '10',
  `skill_axe_exp` int DEFAULT '0',
  `skill_dagger` int DEFAULT '10',
  `skill_dagger_exp` int DEFAULT '0',
  `skill_bow` int DEFAULT '10',
  `skill_bow_exp` int DEFAULT '0',
  `skill_staff` int DEFAULT '10',
  `skill_staff_exp` int DEFAULT '0',
  `skill_shield` int DEFAULT '10',
  `skill_shield_exp` int DEFAULT '0',
  `skill_magick` int DEFAULT '10',
  `skill_magick_exp` int DEFAULT '0',
  `skill_rune` int DEFAULT '10',
  `skill_rune_exp` int DEFAULT '0',
  `skill_protection` int DEFAULT '10',
  `skill_protection_exp` int DEFAULT '0',
  `skill_healing` int DEFAULT '10',
  `skill_healing_exp` int DEFAULT '0',
  `skill_mining` int DEFAULT '10',
  `skill_mining_exp` int DEFAULT '0',
  `skill_blacksmith` int DEFAULT '10',
  `skill_blacksmith_exp` int DEFAULT '0',
  `skill_foraging` int DEFAULT '10',
  `skill_foraging_exp` int DEFAULT '0',
  `skill_alchemy` int DEFAULT '10',
  `skill_alchemy_exp` int DEFAULT '0',
  `skill_fishing` int DEFAULT '10',
  `skill_fishing_exp` int DEFAULT '0',
  `skill_cooking` int DEFAULT '10',
  `skill_cooking_exp` int DEFAULT '0',
  `current_map` int DEFAULT '0',
  `current_map_x` int DEFAULT '0',
  `current_map_y` int DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `enemies`
--

DROP TABLE IF EXISTS `enemies`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enemies` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(25) DEFAULT NULL,
  `health` int DEFAULT '0',
  `mana` int DEFAULT '0',
  `attack` int DEFAULT '0',
  `defence` int DEFAULT '0',
  `magick` int DEFAULT '0',
  `protection` int DEFAULT '0',
  `enemy_type` varchar(25) DEFAULT NULL,
  `experience` int DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `farmiliars`
--

DROP TABLE IF EXISTS `farmiliars`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `farmiliars` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(25) DEFAULT NULL,
  `species` varchar(25) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `item_index`
--

DROP TABLE IF EXISTS `item_index`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `item_index` (
  `id` int NOT NULL AUTO_INCREMENT,
  `item_type` text,
  `item_id` int DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=28 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `locations`
--

DROP TABLE IF EXISTS `locations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `locations` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` text,
  `width` int DEFAULT NULL,
  `height` int DEFAULT NULL,
  `lore` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `mounts`
--

DROP TABLE IF EXISTS `mounts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `mounts` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(25) DEFAULT NULL,
  `species` varchar(25) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `static_equipment`
--

DROP TABLE IF EXISTS `static_equipment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `static_equipment` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` text,
  `attack` int DEFAULT NULL,
  `defence` int DEFAULT NULL,
  `base_bonus` enum('strength','endurance','wisdom','mystic','agility','accuracy','intellect','luck') NOT NULL,
  `base_value` int DEFAULT NULL,
  `combat_bonus` enum('sword','spear','axe','dagger','bow','staff','shield','magick','rune','protection','healing') NOT NULL,
  `combat_value` int DEFAULT NULL,
  `trade_bonus` enum('mining','blacksmith','foraging','alchemy','fishing','cooking') NOT NULL,
  `trade_value` int DEFAULT NULL,
  `cost` int DEFAULT NULL,
  `equipment_type` enum('armor','helmet','amulet','ring','gloves','onhand','offhand','legs','boots') NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=28 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `static_equipment_description`
--

DROP TABLE IF EXISTS `static_equipment_description`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `static_equipment_description` (
  `id` int DEFAULT NULL,
  `description` text,
  `prompt` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `created_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-05-28 12:16:06
