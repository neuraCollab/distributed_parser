-- Create the keyspace
CREATE KEYSPACE IF NOT EXISTS parser
WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1};

-- Use the parser keyspace
USE parser;

-- Create a table for storing task requests
CREATE TABLE IF NOT EXISTS task_requests (
    worker_id text PRIMARY KEY, -- Assuming each worker has a unique ID
    timestamp timestamp,        -- For tracking when the task was requested
    has_task boolean,
    url text
);

-- Create a table for storing task results
CREATE TABLE IF NOT EXISTS task_results (
    worker_id text,
    url text,
    result text,
    timestamp timestamp,        -- For tracking when the result was submitted
    PRIMARY KEY (worker_id, url)
);

-- Create a table for storing submitted tasks
CREATE TABLE IF NOT EXISTS submitted_tasks (
    url text PRIMARY KEY,       -- Unique URL for each task
    success boolean,
    timestamp timestamp         -- For tracking when the task was submitted
);

-- Create a table for storing acknowledgments
CREATE TABLE IF NOT EXISTS result_acks (
    worker_id text,
    url text,
    success boolean,
    timestamp timestamp,        -- For tracking when the acknowledgment was sent
    PRIMARY KEY (worker_id, url)
);
