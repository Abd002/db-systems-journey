SELECT 
    p1.nameFirst || ' (' || p1.nameGiven || ') ' || p1.nameLast AS hof_player_name,
    t.teammate_name AS earliest_teammate_name,
    t.yearID AS earliest_teammate_year
FROM halloffame h
JOIN people p1 ON h.playerID = p1.playerID

JOIN LATERAL (
    SELECT 
        p2.nameFirst || ' (' || p2.nameGiven || ') ' || p2.nameLast AS teammate_name,
        a2.yearID
    FROM appearances a1
    JOIN appearances a2
        ON a1.teamID = a2.teamID
       AND a1.yearID = a2.yearID
       AND a1.playerID != a2.playerID
    JOIN people p2 ON a2.playerID = p2.playerID
    WHERE a1.playerID = h.playerID
    ORDER BY a2.yearID ASC, teammate_name ASC
    LIMIT 1
) t ON TRUE

WHERE h.inducted = 'Y'
ORDER BY hof_player_name ASC
LIMIT 10;
