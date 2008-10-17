/* felica_auth_tbl テーブル新規作成 */

felica_auth_tbl | CREATE TABLE `felica_auth_tbl` (
  `idx` int(11) NOT NULL auto_increment,
  `lastcheckin` datetime NOT NULL,
  `name_id` int(11) NOT NULL,
  `name` varchar(32) NOT NULL,
  `idm_pmm` varchar(33) NOT NULL,
  PRIMARY KEY  (`idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


/* felica_log テーブル新規作成*/

felica_log | CREATE TABLE `felica_log` (
  `idx` int(11) NOT NULL auto_increment,
  `name_id` int(11) NOT NULL,
  `checkin_time` datetime NOT NULL,
  PRIMARY KEY  (`idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

