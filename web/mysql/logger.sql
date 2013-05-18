USE `BIBLEDITDATABASE`;

CREATE TABLE IF NOT EXISTS logger (
  pid int,
  logfile varchar(1024)
) engine = MyISAM;

DROP PROCEDURE IF EXISTS upgrades;
DELIMITER ;;
CREATE PROCEDURE upgrades ()
BEGIN
  DECLARE CONTINUE HANDLER FOR 1060 BEGIN END;
  DECLARE CONTINUE HANDLER FOR 1061 BEGIN END;
  DECLARE CONTINUE HANDLER FOR 1091 BEGIN END;
END;;
CALL upgrades();;
DROP PROCEDURE upgrades;
