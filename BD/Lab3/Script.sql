-- Booking table

DELETE FROM "TourAgency"."Booking";

INSERT INTO "TourAgency"."Booking" ("StartDate", "EndDate", "Status")
SELECT 
    DATE '2022-01-01' + CAST(random() * (DATE '2023-12-31' - DATE '2023-06-01') AS INTEGER) AS StartDate,
    DATE '2023-01-01' + CAST(random() * (DATE '2023-12-31' - DATE '2022-01-01') AS INTEGER) AS EndDate,
    CASE 
        WHEN random() < 0.5 THEN 'Booked'
        ELSE 'Cancelled'
    END AS Status
FROM generate_series(1, 30);

-- SELECT * FROM "TourAgency"."Booking";

-- Client table

DELETE FROM "TourAgency"."Client";

INSERT INTO "TourAgency"."Client" ("FirstName", "SecondName", "ThirdName", "PhoneNumber")
SELECT
    (array['Ivan', 'Alexey', 'Dmitry', 'Sergey', 'Vladimir'])[floor(random()*5)+1] AS FirstName,
    (array['Alexandrovich', 'Ivanovich', 'Petrovich', 'Sergeevich', 'Dmitrievich', 'Vladimirovich', 'Nikolaevich', 'Andreevich', 'Olegovich', 'Anatolievich'])[floor(random()*10)+1] AS SecondName,
    (array['Ivanov', 'Petrov', 'Sidorov', 'Smirnov', 'Kuznetsov', 'Popov', 'Novikov', 'Morozov', 'Volkov', 'Mikhailov'])[floor(random()*10)+1] AS ThirdName,
    concat('+7', trunc(random()*9999999)::text) AS PhoneNumber
FROM generate_series(1, 30);

-- SELECT * FROM "TourAgency"."Client";

-- Hotel table

DELETE FROM "TourAgency"."Hotel";

INSERT INTO "TourAgency"."Hotel" ("Cost", "Name", "Address")
SELECT
    (CAST(floor(random()*100000) AS INTEGER)) AS Cost,
    (array['Hilton', 'Marriott', 'Ritz-Carlton', 'Four Seasons', 'InterContinental', 'Sheraton', 'Westin', 'Hyatt', 'Holiday Inn', 'Radisson Blu'])[floor(random()*10)+1] AS Name,
    concat('Address ', floor(random()*1000)+1, ', City') AS Address
FROM generate_series(1, 30);

-- SELECT * FROM "TourAgency"."Hotel";

-- Payment table

DELETE FROM "TourAgency"."Payment";

INSERT INTO "TourAgency"."Payment" ("Date", "Amount", "Discount")
SELECT 
    DATE '2022-01-01' + CAST(random() * (DATE '2023-12-31' - DATE '2022-01-01') AS INTEGER) AS Date,
    (CAST(floor(random()*100000) AS INTEGER)) AS Amount,
	(CAST(floor(random()*70) AS SMALLINT)) AS Discount
FROM generate_series(1, 30);

-- SELECT * FROM "TourAgency"."Payment";

-- Tour table

DELETE FROM "TourAgency"."Tour";

INSERT INTO "TourAgency"."Tour" ("Days", "Name", "Type")
SELECT 
    (CAST(floor(random()*60) AS SMALLINT)) AS Days,
	((array['Russia', 'Belarus', 'USA', 'Canada', 'Poland'])[floor(random()*5)+1]) AS Name,
	((array['Hot', 'Non-hot'])[floor(random()*2)+1]) AS Type
FROM generate_series(1, 30);

-- SELECT * FROM "TourAgency"."Tour";

-- Voucher table

DELETE FROM "TourAgency"."Voucher";

INSERT INTO "TourAgency"."Voucher" ("StartDate", "EndDate", "Status")
SELECT 
    DATE '2022-01-01' + CAST(random() * (DATE '2023-12-31' - DATE '2023-06-01') AS INTEGER) AS StartDate,
    DATE '2023-01-01' + CAST(random() * (DATE '2023-12-31' - DATE '2022-01-01') AS INTEGER) AS EndDate,
    CASE 
        WHEN random() < 0.5 THEN 'Booked'
        ELSE 'Cancelled'
    END AS Status
FROM generate_series(1, 30);

-- SELECT * FROM "TourAgency"."Voucher";