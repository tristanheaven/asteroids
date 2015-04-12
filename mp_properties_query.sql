SELECT `number`,`taxonomy_class` FROM `properties`
WHERE `taxonomy_class` IN ('M','S','C')
AND `number` IS NOT NULL
