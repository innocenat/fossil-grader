-- phpMyAdmin SQL Dump
-- version 2.11.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Apr 04, 2008 at 09:07 PM
-- Server version: 5.0.27
-- PHP Version: 5.2.4

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `posn`
--

-- --------------------------------------------------------

--
-- Table structure for table `grd_queue`
--

CREATE TABLE IF NOT EXISTS `grd_queue` (
  `q_id` int(10) unsigned NOT NULL auto_increment,
  `user_id` varchar(10) NOT NULL default '',
  `prob_id` varchar(10) NOT NULL default '',
  `sub_num` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`q_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `grd_status`
--

CREATE TABLE IF NOT EXISTS `grd_status` (
  `user_id` varchar(10) NOT NULL default '',
  `prob_id` varchar(10) NOT NULL default '',
  `res_id` int(10) unsigned NOT NULL default '0',
  `score` int(10) unsigned default '0',
  `compiler_msg` text,
  `grading_msg` varchar(100) default NULL,
  PRIMARY KEY  (`user_id`,`prob_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `prob_info`
--

CREATE TABLE IF NOT EXISTS `prob_info` (
  `prob_id` varchar(10) NOT NULL default '',
  `name` varchar(100) NOT NULL default '',
  `avail` char(1) NOT NULL default '',
  `prob_order` int(10) unsigned default NULL,
  PRIMARY KEY  (`prob_id`),
  KEY `ordering` (`prob_order`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `res_desc`
--

CREATE TABLE IF NOT EXISTS `res_desc` (
  `res_id` int(10) unsigned NOT NULL auto_increment,
  `res_text` varchar(45) NOT NULL default '',
  PRIMARY KEY  (`res_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=5 ;

-- --------------------------------------------------------


INSERT INTO `res_desc` (`res_id`, `res_text`) VALUES
(1, 'in queue'),
(2, 'grading'),
(3, 'accepted'),
(4, 'rejected');

--
-- Table structure for table `submission`
--

CREATE TABLE IF NOT EXISTS `submission` (
  `user_id` varchar(10) NOT NULL default '',
  `prob_id` varchar(10) NOT NULL default '',
  `sub_num` int(11) NOT NULL default '0',
  `time` datetime default '0000-00-00 00:00:00',
  `code` mediumtext NOT NULL,
  PRIMARY KEY  (`user_id`,`prob_id`,`sub_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `user_info`
--

CREATE TABLE IF NOT EXISTS `user_info` (
  `user_id` varchar(10) NOT NULL default '',
  `name` varchar(100) NOT NULL default '',
  `passwd` varchar(10) NOT NULL default '',
  `grp` varchar(10) default NULL,
  `type` char(1) NOT NULL default '',
  PRIMARY KEY  (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `user_info`
--

INSERT INTO `user_info` (`user_id`, `name`, `passwd`, `grp`, `type`) VALUES
('root', 'Administrator', 'serverroot', NULL, 'A');
