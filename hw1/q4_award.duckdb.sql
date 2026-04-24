WITH player_awards AS (
    SELECT 
        a.teamID,
        a.yearID
    FROM awardsplayers ap
    JOIN appearances a
        ON ap.playerID = a.playerID
       AND ap.yearID = a.yearID
    GROUP BY a.teamID, a.yearID
    HAVING COUNT(DISTINCT ap.playerID) > 5
),

manager_years AS (
    SELECT DISTINCT yearID
    FROM awardsmanagers
),

event_E AS (
    SELECT pa.teamID, pa.yearID
    FROM player_awards pa
    JOIN manager_years my
        ON pa.yearID = my.yearID
),

active_teams AS (
    SELECT t.teamID, t.yearID, t.name, l.league
    FROM teams t
    JOIN leagues l ON t.lgID = l.lgID
    WHERE l.active = 'Y'
)

SELECT 
    at.league,
    at.name AS team_name,
    COUNT(*) AS distinct_years
FROM event_E e
JOIN active_teams at
    ON e.teamID = at.teamID
   AND e.yearID = at.yearID
GROUP BY at.league, at.name
HAVING COUNT(*) > 1
ORDER BY distinct_years DESC, team_name ASC;
