SELECT
    p.nameGiven,
    a.teamID,
    COUNT(DISTINCT ap.yearID) AS distinct_years
FROM awardsplayers ap
JOIN appearances a 
    ON ap.playerID = a.playerID 
   AND ap.yearID = a.yearID
JOIN teams t 
    ON a.teamID = t.teamID 
   AND a.yearID = t.yearID
JOIN people p 
    ON p.playerID = ap.playerID
WHERE ap.awardID LIKE '%Gold Glove%'
  AND ap.yearID > 1999
  AND t.lgID IN (
      SELECT lgID FROM leagues WHERE active = 'Y'
  )
  AND a.G_batting > (
      SELECT AVG(a2.G_batting)
      FROM appearances a2
      WHERE a2.teamID = a.teamID
        AND a2.yearID > 1999
  )
GROUP BY p.nameGiven, a.teamID
ORDER BY distinct_years DESC, p.nameGiven ASC
LIMIT 10;
