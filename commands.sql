create database if not exists social_network_db;

create table if not exists chats_table
(
    id int auto_increment   primary key,
    sender_id int(100)      not null,
    receiver_id int(100)    not null
);

create index if not exists sender_id on chats_table(sender_id);
create index if not exists receiver_id on chats_table(receiver_id);



create table if not exists messages_table
(
    id int auto_increment                           primary key,
    chat_id int(100)                                not null,
    user_sernder_id int(100)                        not null,
    content varchar(256) collate utf8_unicode_ci    not null
);
create index if not exists message_chat_id_index on messages_table(chat_id);



create table if not exists users_table
(
    id int auto_increment                               primary key,
    first_name varchar(256) collate utf8_unicode_ci     not null,
    last_name varchar(256) collate utf8_unicode_ci      not null,
    user_login varchar(64) collate utf8_unicode_ci      not null,
    user_password varchar(64) collate utf8_unicode_ci   not null,
    email varchar(256) collate utf8_unicode_ci          not null,
    phone_number varchar(11) collate utf8_unicode_ci    null,
    birth_date varchar(10) collate utf8_unicode_ci      null
);
create index if not exists user_first_name_index on users_table(first_name);
create index if not exists user_last_name_index on users_table(last_name);



create table if not exists posts_table
(
    id int auto_increment                               primary key,
    user_id int(100)                                    not null,
    content varchar(256) collate utf8_unicode_ci        not null,
    creation_date varchar(10) collate utf8_unicode_ci   not null
);
create index if not exists post_user_id_index on posts_table(user_id);



insert into social_network_db.users_table(first_name, last_name, user_login, user_password, email) values (
    'test_user_name1',
    'test_user_surname1',
    'test_user_login1',
    'test_user_password1',
    'email1@user.test1'
);
insert into social_network_db.users_table(first_name, last_name, user_login, user_password, email) values (
    'test_user_name2',
    'test_user_surname2',
    'test_user_login2',
    'test_user_password2',
    'email2@user.test2'
);

insert into social_network_db.chats_table(sender_id, receiver_id) values (1, 2);
insert into social_network_db.chats_table(sender_id, receiver_id) values (2, 1);

insert into social_network_db.posts_table(user_id, content, creation_date) values (1, 'post test content', '30.04.2023');